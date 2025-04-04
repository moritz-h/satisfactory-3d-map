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

    auto save_version_stack_pusher = ar.pushSaveVersion(mSaveHeader.SaveVersion);

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

    save_version_stack_pusher.reset();

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
    class_objects_map_.clear();
    path_objects_map_.clear();
    object_level_map_.clear();

    // Determine total object number
    std::size_t num_objects = mPersistentAndRuntimeData.SaveObjects.size();
    for (const auto& lvl : mPerLevelDataMap.Values) {
        num_objects += lvl.SaveObjects.size();
    }

    all_save_objects_.reserve(num_objects);
    // Do not reserve class map, number of classes is limited
    path_objects_map_.reserve(num_objects); // number of paths is close to number of objects
    object_level_map_.reserve(num_objects);

    for (int lvlIdx = 0; lvlIdx < mPerLevelDataMap.Values.size(); lvlIdx++) {
        for (const auto& obj : mPerLevelDataMap.Values[lvlIdx].SaveObjects) {
            addObjectAccess(obj, lvlIdx);
        }
    }
    for (const auto& obj : mPersistentAndRuntimeData.SaveObjects) {
        addObjectAccess(obj, -1);
    }
}
void SatisfactorySave::SaveGame::addObjectAccess(const SaveObjectPtr& obj, int level) {
    all_save_objects_.emplace_back(obj);
    class_objects_map_[obj->baseHeader().ClassName].emplace_back(obj);
    path_objects_map_[obj->baseHeader().Reference.PathName].emplace_back(obj);
    object_level_map_.emplace(obj, level);
}

bool SatisfactorySave::SaveGame::addObject(const SaveObjectPtr& obj, int level) {
    if (level == -1) {
        mPersistentAndRuntimeData.SaveObjects.push_back(obj);
    } else if (level >= 0 && level < mPerLevelDataMap.size()) {
        mPerLevelDataMap.Values[level].SaveObjects.push_back(obj);
    } else {
        return false;
    }
    addObjectAccess(obj, level);
    return true;
}

bool SatisfactorySave::SaveGame::addObjects(const SaveObjectList& objects, int level) {
    if (level < -1 || level >= static_cast<int>(mPerLevelDataMap.size())) {
        return false;
    }
    for (const auto& obj : objects) {
        addObject(obj, level);
    }
    return true;
}

bool SatisfactorySave::SaveGame::removeObject(const SaveObjectPtr& obj) {
    if (!object_level_map_.contains(obj)) {
        return false;
    }
    const auto level = object_level_map_.at(obj);
    if (level == -1) {
        std::erase(mPersistentAndRuntimeData.SaveObjects, obj);
    } else if (level >= 0 && level < mPerLevelDataMap.size()) {
        std::erase(mPerLevelDataMap.Values[level].SaveObjects, obj);
    }
    std::erase(all_save_objects_, obj);
    std::erase(class_objects_map_[obj->baseHeader().ClassName], obj);
    std::erase(path_objects_map_[obj->baseHeader().Reference.PathName], obj);
    object_level_map_.erase(obj);
    return true;
}

bool SatisfactorySave::SaveGame::removeObjects(const SaveObjectList& objects) {
    // Check if all objects exist. We want to delete all or nothing.
    for (const auto& obj : objects) {
        if (!object_level_map_.contains(obj)) {
            return false;
        }
    }
    for (const auto& obj : objects) {
        removeObject(obj);
    }
    return true;
}
