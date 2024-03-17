#include "GameTypes/Save/SaveGame.h"

#include <functional>
#include <stdexcept>
#include <utility>

#include "GameTypes/Save/ChunkHelper.h"
#include "GameTypes/Save/SaveActor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "IO/ZlibUtils.h"
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
                return SatisfactorySave::natLessCaseInsensitive(a->Reference.PathName, b->Reference.PathName);
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
    fileAr << header_;
    if (header_.SaveHeaderVersion != FSaveHeader::LatestVersion) {
        throw std::runtime_error("Unknown Save-Header Version: " + std::to_string(header_.SaveHeaderVersion));
    }
    if (header_.SaveVersion < 41) {
        throw std::runtime_error(
            "Save version must be at least 41 (Update 8). Found old version: " + std::to_string(header_.SaveVersion));
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

    // Validate blob size
    if (file_data_blob_size - sizeof(int64_t) != ar.read<int64_t>()) {
        throw std::runtime_error("Bad blob size!");
    }
    TIME_MEASURE_END("toStream");

    // ValidationData
    ar << ValidationData;

    // Parse levels
    TIME_MEASURE_START("Levels");
    const auto numLevels = ar.read<int32_t>();
    per_level_data_.reserve(numLevels);
    for (int32_t l = 0; l < numLevels; l++) {
        PerLevelData level;
        ar << level.level_name;
        parseTOCBlob(ar, level.save_objects, level.destroyed_actors_TOC, level.has_destroyed_actors_TOC);
        parseDataBlob(ar, level.save_objects);
        ar << level.destroyed_actors;

        per_level_data_.emplace_back(std::move(level));
    }
    TIME_MEASURE_END("Levels");

    TIME_MEASURE_START("PersistentLevel");
    bool dummy = false;
    parseTOCBlob(ar, persistent_and_runtime_data_.save_objects, persistent_and_runtime_data_.destroyed_actors_TOC,
        dummy);
    parseDataBlob(ar, persistent_and_runtime_data_.save_objects);
    ar.read_assert_zero<int32_t>(); // LevelToDestroyedActorsMap always zero
    TIME_MEASURE_END("PersistentLevel");

    // Parse unresolved data
    TIME_MEASURE_START("Unresolved");
    ar << unresolved_world_save_data_;
    TIME_MEASURE_END("Unresolved");

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

    // ValidationData
    ar << ValidationData;

    // Save levels
    ar.write(static_cast<int32_t>(per_level_data_.size()));

    for (auto& level : per_level_data_) {
        ar << level.level_name;
        saveTOCBlob(ar, level.save_objects, level.destroyed_actors_TOC, level.has_destroyed_actors_TOC);
        saveDataBlob(ar, level.save_objects);
        ar << level.destroyed_actors;
    }

    saveTOCBlob(ar, persistent_and_runtime_data_.save_objects, persistent_and_runtime_data_.destroyed_actors_TOC, true);
    saveDataBlob(ar, persistent_and_runtime_data_.save_objects);
    ar.write<int32_t>(0); // LevelToDestroyedActorsMap always zero

    ar << unresolved_world_save_data_;

    // Store size
    auto blob_size = ar.tell();
    ar.seek(0);
    ar.write(static_cast<int64_t>(blob_size - sizeof(int64_t)));

    // Write to file

    // Open file
    OFStreamArchive fileAr(filepath);

    // Write header
    fileAr << header_;

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

void SatisfactorySave::SaveGame::parseTOCBlob(IStreamArchive& ar, SaveObjectList& saveObjects,
    std::vector<FObjectReferenceDisc>& destroyedActorsTOC, bool& has_destroyedActorsTOC) {
    const auto TOC_size = ar.read<int64_t>();
    const auto TOC_pos = ar.tell();

    // Parse objects
    TIME_MEASURE_START("ObjHead");
    const auto num_objects = ar.read<int32_t>();
    saveObjects.reserve(num_objects);
    for (int32_t i = 0; i < num_objects; i++) {
        saveObjects.emplace_back(SaveObjectBase::create(ar));
    }
    TIME_MEASURE_END("ObjHead");

    // TODO ???
    has_destroyedActorsTOC = ar.tell() - TOC_pos < TOC_size;
    if (has_destroyedActorsTOC) {
        ar << destroyedActorsTOC;
    }

    if (ar.tell() - TOC_pos != TOC_size) {
        throw std::runtime_error("Invalid size of TOCBlob!");
    }
}

void SatisfactorySave::SaveGame::parseDataBlob(IStreamArchive& ar, SaveObjectList& saveObjects) {
    const auto data_size = ar.read<int64_t>();
    const auto data_pos = ar.tell();

    // Parse object properties
    TIME_MEASURE_START("ObjProp");
    const auto num_object_data = ar.read<int32_t>();
    if (saveObjects.size() != static_cast<std::size_t>(num_object_data)) {
        throw std::runtime_error("Bad number of object data!");
    }
    // TODO: we can potentially do this in parallel, but this requires a thread pool and worker queue.
    for (int32_t i = 0; i < num_object_data; i++) {
        // TODO
        ar << saveObjects[i]->unk1;
        ar << saveObjects[i]->unk2;

        // Check stream pos to validate parser.
        const auto length = ar.read<int32_t>();
        auto pos_before = ar.tell();
        saveObjects[i]->serializeProperties(ar, length);
        auto pos_after = ar.tell();
        if (pos_after - pos_before != length) {
            throw std::runtime_error("Error parsing object data!");
        }
    }
    TIME_MEASURE_END("ObjProp");

    if (ar.tell() - data_pos != data_size) {
        throw std::runtime_error("Invalid size of DataBlob!");
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
    std::size_t num_objects = persistent_and_runtime_data_.save_objects.size();
    for (const auto& lvl : per_level_data_) {
        num_objects += lvl.save_objects.size();
    }

    all_save_objects_.resize(num_objects);
    std::size_t globalIdx = 0;
    for (int lvlIdx = 0; lvlIdx < per_level_data_.size(); lvlIdx++) {
        for (std::size_t listIdx = 0; listIdx < per_level_data_[lvlIdx].save_objects.size(); listIdx++) {
            const auto& obj = per_level_data_[lvlIdx].save_objects[listIdx];
            all_save_objects_[globalIdx] = obj;
            info_map_.emplace(obj, SaveObjectInfo{lvlIdx, listIdx, globalIdx});
            globalIdx++;
        }
    }
    for (std::size_t listIdx = 0; listIdx < persistent_and_runtime_data_.save_objects.size(); listIdx++) {
        const auto& obj = persistent_and_runtime_data_.save_objects[listIdx];
        all_save_objects_[globalIdx] = obj;
        info_map_.emplace(obj, SaveObjectInfo{-1, listIdx, globalIdx});
        globalIdx++;
    }

    TIME_MEASURE_START("toTree");
    // Generate object structures for fast access
    level_root_nodes_.resize(per_level_data_.size());
    for (std::size_t i = 0; i < per_level_data_.size(); i++) {
        initAccessStructures(per_level_data_[i].save_objects, level_root_nodes_[i]);
    }

    initAccessStructures(persistent_and_runtime_data_.save_objects, persistent_and_runtime_root_node_);
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
        path_objects_map_[obj->Reference.PathName].emplace_back(obj);

        // Store objects into a tree structure for access by class
        std::reference_wrapper<SaveNode> n = rootNode;
        std::reference_wrapper<SaveNode> a_n = all_root_node_;
        for (const auto& s : splitPathName(obj->ClassName)) {
            n = n.get().childNodes[s];
            a_n = a_n.get().childNodes[s];
        }
        n.get().objects.emplace_back(obj);
        a_n.get().objects.emplace_back(obj);
    }
}

void SatisfactorySave::SaveGame::saveTOCBlob(OStreamArchive& ar, SaveObjectList& saveObjects,
    std::vector<FObjectReferenceDisc>& destroyedActorsTOC, bool has_destroyedActorsTOC) {
    auto pos_size = ar.tell();
    ar.write<int64_t>(0);

    auto pos_before = ar.tell();

    // Write objects
    ar.write(static_cast<int32_t>(saveObjects.size()));
    for (const auto& obj : saveObjects) {
        ar.write(obj->isActor());
        ar << *obj;
    }

    if (has_destroyedActorsTOC) {
        ar << destroyedActorsTOC;
    }

    auto pos_after = ar.tell();

    ar.seek(pos_size);
    ar.write(static_cast<int64_t>(pos_after - pos_before));
    ar.seek(pos_after);
}

void SatisfactorySave::SaveGame::saveDataBlob(OStreamArchive& ar, SaveObjectList& saveObjects) {
    auto blob_pos_size = ar.tell();
    ar.write<int64_t>(0);

    auto blob_pos_before = ar.tell();

    // Write object properties
    ar.write(static_cast<int32_t>(saveObjects.size()));
    for (const auto& obj : saveObjects) {
        ar << obj->unk1;
        ar << obj->unk2;

        auto pos_size = ar.tell();
        ar.write<int32_t>(0);

        auto pos_before = ar.tell();
        obj->serializeProperties(ar, 0);
        auto pos_after = ar.tell();

        ar.seek(pos_size);
        ar.write(static_cast<int32_t>(pos_after - pos_before));
        ar.seek(pos_after);
    }

    auto blob_pos_after = ar.tell();

    ar.seek(blob_pos_size);
    ar.write(static_cast<int64_t>(blob_pos_after - blob_pos_before));
    ar.seek(blob_pos_after);
}

bool SatisfactorySave::SaveGame::addObject(const SaveObjectPtr& obj, int level) {
    return addObjects({obj}, level);
}

bool SatisfactorySave::SaveGame::addObjects(const SaveObjectList& objects, int level) {
    if (level < -1 || level >= static_cast<int>(per_level_data_.size())) {
        return false;
    }
    for (const auto& obj : objects) {
        if (level == -1) {
            persistent_and_runtime_data_.save_objects.push_back(obj);
        } else if (level >= 0 && level < per_level_data_.size()) {
            per_level_data_[level].save_objects.push_back(obj);
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
        auto& list = (level == -1) ? persistent_and_runtime_data_.save_objects : per_level_data_.at(level).save_objects;
        std::sort(remove_ids.begin(), remove_ids.end(), std::greater<>());
        for (const auto& id : remove_ids) {
            list.erase(list.begin() + id);
        }
    }
    // TODO full reinit is very slow
    initAccessStructures();
    return true;
}
