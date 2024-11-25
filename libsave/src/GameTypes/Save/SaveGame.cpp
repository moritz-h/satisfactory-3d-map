#include "GameTypes/Save/SaveGame.h"

#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

#include "ChunkHelper.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "IO/ZlibUtils.h"
#include "SerializationUtils.h"
#include "Utils/StringUtils.h"
#include "Utils/TimeMeasure.h"

namespace {
    void countAndSortObjects(SatisfactorySave::SaveGame::SaveNode& node) {
        node.numObjects = 0;
        node.numActors = 0;
        for (auto& child : node.childNodes) {
            countAndSortObjects(child.second);
            node.numObjects += child.second.numObjects;
            node.numActors += child.second.numActors;
        }
        if (!node.objects.empty()) {
            for (const auto& obj : node.objects) {
                if (obj->isActor()) {
                    node.numActors++;
                } else {
                    node.numObjects++;
                }
            }
            std::stable_sort(node.objects.begin(), node.objects.end(), [](const auto& a, const auto& b) {
                return SatisfactorySave::natLessCaseInsensitive(a->baseHeader().Reference.PathName,
                    b->baseHeader().Reference.PathName);
            });
        }
    }
} // namespace

SatisfactorySave::SaveGame::SaveGame(const std::filesystem::path& filepath) {
    TIME_MEASURE_CLEAR();
    TIME_MEASURE_START("Total");

    // Open file
    TIME_MEASURE_START("Open");
    IFStreamArchive fileAr(filepath);
    TIME_MEASURE_END("Open");

    // Serialize header
    TIME_MEASURE_START("Header");
    fileAr << mSaveHeader;
    if (mSaveHeader.SaveHeaderVersion != FSaveHeader::LatestVersion) {
        throw std::runtime_error("Unknown Save-Header Version: " + std::to_string(mSaveHeader.SaveHeaderVersion));
    }
    if (mSaveHeader.SaveVersion < 41) {
        throw std::runtime_error("Save version must be at least 41 (Update 8). Found old version: " +
                                 std::to_string(mSaveHeader.SaveVersion));
    }
    TIME_MEASURE_END("Header");

    // TODO verify md5 hash

    // Read and decompress chunks
    // Start with reading chunk headers and compressed buffers. Then the size of the decompressed blob is known in
    // advance and be allocated all at once. Decompression then can write directory to this final buffer without
    // any reallocation. Further the decompression of all chunks can run completely in parallel.
    TIME_MEASURE_START("Chunk");
    auto file_data_blob = decompressChunks(fileAr);
    TIME_MEASURE_END("Chunk");

    // Store size and init memory stream
    TIME_MEASURE_START("toStream");
    const auto file_data_blob_size = file_data_blob->size();
    IStreamArchive ar(std::make_unique<MemIStream>(std::move(file_data_blob)));

    // Store header SaveVersion as first stack entry.
    auto save_version_stack_pusher = ar.pushSaveVersion(mSaveHeader.SaveVersion);

    // Validate blob size
    if (file_data_blob_size - sizeof(int64_t) != ar.read<int64_t>()) {
        throw std::runtime_error("Bad blob size!");
    }
    TIME_MEASURE_END("toStream");

    // ValidationData
    ar << SaveGameValidationData;

    // Parse levels
    TIME_MEASURE_START("PerLevelData");
    ar << mPerLevelDataMap;
    TIME_MEASURE_END("PerLevelData");

    TIME_MEASURE_START("PersistentAndRuntimeData");
    ar << mPersistentAndRuntimeData;
    TIME_MEASURE_END("PersistentAndRuntimeData");

    // Parse unresolved data
    TIME_MEASURE_START("UnresolvedWorldSaveData");
    ar << mUnresolvedWorldSaveData;
    TIME_MEASURE_END("UnresolvedWorldSaveData");

    // Validate stream is completely read
    if (static_cast<long>(file_data_blob_size) != ar.tell()) {
        throw std::runtime_error("Error parsing save file: Size check after parsing failed!");
    }

    initAccessStructures();

    TIME_MEASURE_END("Total");
    TIME_MEASURE_PRINT();
}

void SatisfactorySave::SaveGame::save(const std::filesystem::path& filepath) {
    // Serialize data to blob
    OMemStreamArchive ar(std::make_unique<MemOStream>());

    // Size placeholder
    ar.write<int64_t>(0);

    ar << SaveGameValidationData;
    ar << mPerLevelDataMap;
    ar << mPersistentAndRuntimeData;
    ar << mUnresolvedWorldSaveData;

    // Store size
    auto blob_size = ar.tell();
    ar.seek(0);
    ar.write(static_cast<int64_t>(blob_size - sizeof(int64_t)));

    // Write to file

    // Open file
    OFStreamArchive fileAr(filepath);

    // Write header
    fileAr << mSaveHeader;

    // Split blob into chunks
    uint64_t blob_pos = 0;
    const char* blob_buffer = ar.data().data();

    while (blob_size > 0) {
        // Compress chunk
        int64_t chunk_size = std::min(static_cast<int64_t>(blob_size), ChunkHeader::COMPRESSION_CHUNK_SIZE);
        std::vector<char> chunk_uncompressed{blob_buffer + blob_pos, blob_buffer + blob_pos + chunk_size};
        std::vector<char> chunk_compressed = zlibCompress(chunk_uncompressed);

        blob_pos += chunk_size;
        blob_size -= chunk_size;

        // Chunk header
        ChunkHeader chunkHeader(static_cast<int64_t>(chunk_compressed.size()), chunk_size);
        fileAr << chunkHeader;

        fileAr.write_buffer(chunk_compressed);
    }
}

void SatisfactorySave::SaveGame::initAccessStructures() {
    // Cleanup
    all_save_objects_.clear();
    info_map_.clear();
    path_objects_map_.clear();
    level_root_nodes_.clear();
    persistent_and_runtime_root_node_ = SaveNode();
    all_root_node_ = SaveNode();

    // Generate ids
    std::size_t num_objects = mPersistentAndRuntimeData.SaveObjects.size();
    for (const auto& lvl : mPerLevelDataMap.Values) {
        num_objects += lvl.SaveObjects.size();
    }

    all_save_objects_.resize(num_objects);
    std::size_t globalIdx = 0;
    for (int lvlIdx = 0; lvlIdx < mPerLevelDataMap.Values.size(); lvlIdx++) {
        for (std::size_t listIdx = 0; listIdx < mPerLevelDataMap.Values[lvlIdx].SaveObjects.size(); listIdx++) {
            const auto& obj = mPerLevelDataMap.Values[lvlIdx].SaveObjects[listIdx];
            all_save_objects_[globalIdx] = obj;
            info_map_.emplace(obj, SaveObjectInfo{lvlIdx, listIdx, globalIdx});
            globalIdx++;
        }
    }
    for (std::size_t listIdx = 0; listIdx < mPersistentAndRuntimeData.SaveObjects.size(); listIdx++) {
        const auto& obj = mPersistentAndRuntimeData.SaveObjects[listIdx];
        all_save_objects_[globalIdx] = obj;
        info_map_.emplace(obj, SaveObjectInfo{-1, listIdx, globalIdx});
        globalIdx++;
    }

    TIME_MEASURE_START("toTree");
    // Generate object structures for fast access
    level_root_nodes_.resize(mPerLevelDataMap.size());
    for (std::size_t i = 0; i < mPerLevelDataMap.size(); i++) {
        initAccessStructures(mPerLevelDataMap.Values[i].SaveObjects, level_root_nodes_[i]);
    }

    initAccessStructures(mPersistentAndRuntimeData.SaveObjects, persistent_and_runtime_root_node_);
    TIME_MEASURE_END("toTree");

    // Count number of child objects in tree
    TIME_MEASURE_START("Count");
    countAndSortObjects(all_root_node_);
    countAndSortObjects(persistent_and_runtime_root_node_);
    for (auto& node : level_root_nodes_) {
        countAndSortObjects(node);
    }
    TIME_MEASURE_END("Count");
}

void SatisfactorySave::SaveGame::initAccessStructures(const SaveObjectList& saveObjects, SaveNode& rootNode) {
    for (const auto& obj : saveObjects) {
        // Store objects into map for access by name
        path_objects_map_[obj->baseHeader().Reference.PathName].emplace_back(obj);

        // Store objects into a tree structure for access by class
        std::reference_wrapper<SaveNode> n = rootNode;
        std::reference_wrapper<SaveNode> a_n = all_root_node_;
        for (const auto& s : splitPathName(obj->baseHeader().ClassName)) {
            n = n.get().childNodes[s];
            a_n = a_n.get().childNodes[s];
        }
        n.get().objects.emplace_back(obj);
        a_n.get().objects.emplace_back(obj);
    }
}

SatisfactorySave::SaveObjectList SatisfactorySave::SaveGame::getObjectsByClass(const std::string& className) {
    std::reference_wrapper<SaveNode> n = all_root_node_;
    for (const auto& it : splitPathName(className)) {
        if (!n.get().childNodes.contains(it)) {
            return {};
        }
        n = n.get().childNodes[it];
    }
    return n.get().objects;
}

bool SatisfactorySave::SaveGame::addObject(const SaveObjectPtr& obj, int level) {
    return addObjects({obj}, level);
}

bool SatisfactorySave::SaveGame::addObjects(const SaveObjectList& objects, int level) {
    if (level < -1 || level >= static_cast<int>(mPerLevelDataMap.size())) {
        return false;
    }
    for (const auto& obj : objects) {
        if (level == -1) {
            mPersistentAndRuntimeData.SaveObjects.push_back(obj);
        } else if (level >= 0 && level < mPerLevelDataMap.size()) {
            mPerLevelDataMap.Values[level].SaveObjects.push_back(obj);
        }
    }
    // TODO full reinit is very slow
    initAccessStructures();
    return true;
}

bool SatisfactorySave::SaveGame::removeObject(const SaveObjectPtr& obj) {
    return removeObjects({obj});
}

bool SatisfactorySave::SaveGame::removeObjects(const SaveObjectList& objects) {
    // ID's in info objects are not updated between deletions, therefore delete in reversed ID order.
    std::unordered_map<int, std::vector<std::size_t>> remove_ids_map;
    for (const auto& obj : objects) {
        if (!info_map_.contains(obj)) {
            return false;
        }
        const auto info = info_map_.at(obj);
        remove_ids_map[info.level_idx].push_back(info.level_list_idx);
    }
    for (auto& [level, remove_ids] : remove_ids_map) {
        auto& list =
            (level == -1) ? mPersistentAndRuntimeData.SaveObjects : mPerLevelDataMap.Values.at(level).SaveObjects;
        std::sort(remove_ids.begin(), remove_ids.end(), std::greater<>());
        for (const auto& id : remove_ids) {
            list.erase(list.begin() + id);
        }
    }
    // TODO full reinit is very slow
    initAccessStructures();
    return true;
}
