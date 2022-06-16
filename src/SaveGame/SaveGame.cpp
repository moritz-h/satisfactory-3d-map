#include "SaveGame.h"

#include <functional>
#include <stdexcept>
#include <utility>

#include "GameTypes/SaveObjects/SaveActor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "IO/ZlibUtils.h"
#include "Utils/StringUtils.h"
#include "Utils/TimeMeasure.h"

namespace {
    void countObjects(Satisfactory3DMap::SaveGame::SaveNode& node) {
        node.numObjects = 0;
        node.numActors = 0;
        for (auto& child : node.childNodes) {
            countObjects(child.second);
            node.numObjects += child.second.numObjects;
            node.numActors += child.second.numActors;
        }
        for (const auto& obj : node.objects) {
            if (obj.second->type() == 1) {
                node.numActors++;
            } else {
                node.numObjects++;
            }
        }
    }
} // namespace

Satisfactory3DMap::SaveGame::SaveGame(const std::filesystem::path& filepath) {
    TIME_MEASURE_CLEAR();

    // Open file
    TIME_MEASURE_START("Open");
    IFStreamArchive fileAr(filepath);
    TIME_MEASURE_END("Open");

    // Serialize header
    TIME_MEASURE_START("Header");
    fileAr << header_;
    TIME_MEASURE_END("Header");

    // Read and decompress chunks
    // Start with reading chunk headers and compressed buffers. Then the size of the decompressed blob is known in
    // advance and be allocated all at once. Decompression then can write directory to this final buffer without
    // any reallocation. Further the decompression of all chunks can run completely in parallel.
    TIME_MEASURE_START("Chunk");
    std::vector<ChunkInfo> chunk_list;
    std::size_t total_decompressed_size = 0;
    while (fileAr.tell() < fileAr.size()) {
        ChunkHeader chunk_header;
        fileAr << chunk_header;
        chunk_list.emplace_back(chunk_header, fileAr.read_vector<char>(chunk_header.compressedSize()),
            total_decompressed_size);
        total_decompressed_size += chunk_header.uncompressedSize();
    }

    // Create a buffer with the total decompressed size.
    auto file_data_blob = std::make_unique<std::vector<char>>(total_decompressed_size);

    // Decompress in parallel.
    // OpenMP requires a signed integer type for loop.
    const int64_t size = static_cast<int64_t>(chunk_list.size());
#pragma omp parallel for
    for (int64_t i = 0; i < size; i++) {
        const ChunkInfo& chunk = chunk_list[i];
        char* decompressed_buffer_ptr = file_data_blob->data() + chunk.decompressed_offset;
        zlibUncompress(decompressed_buffer_ptr, chunk.header.uncompressedSize(), chunk.compressed_chunk.data(),
            chunk.compressed_chunk.size());
    }
    TIME_MEASURE_END("Chunk");

    // Store size and init memory stream
    TIME_MEASURE_START("toStream");
    const auto file_data_blob_size = file_data_blob->size();
    IStreamArchive ar(std::make_unique<MemIStream>(std::move(file_data_blob)));

    // Validate blob size
    if (static_cast<int32_t>(file_data_blob_size - sizeof(int32_t)) != ar.read<int32_t>()) {
        throw std::runtime_error("Bad blob size!");
    }
    TIME_MEASURE_END("toStream");

    // Parse levels
    TIME_MEASURE_START("Levels");
    const auto numLevels = ar.read<int32_t>();
    level_data_.reserve(numLevels);
    for (int32_t l = 0; l < numLevels; l++) {
        LevelData level;
        ar << level.level_name;
        parseTOCBlob(ar, level.save_objects, level.destroyed_actors_TOC);
        parseDataBlob(ar, level.save_objects);
        ar << level.destroyed_actors;

        level_data_.emplace_back(std::move(level));
    }
    TIME_MEASURE_END("Levels");

    TIME_MEASURE_START("PersistentLevel");
    parseTOCBlob(ar, save_objects_, destroyed_actors_toc_);
    parseDataBlob(ar, save_objects_);

    // TODO
    ar.read_assert_zero<int32_t>();
    TIME_MEASURE_END("PersistentLevel");

    // Parse unresolved data
    TIME_MEASURE_START("Unresolved");
    ar << unresolved_world_save_data_;
    TIME_MEASURE_END("Unresolved");

    // Validate stream is completely read
    if (static_cast<long>(file_data_blob_size) != ar.tell()) {
        throw std::runtime_error("Error parsing save file: Size check after parsing failed!");
    }

    TIME_MEASURE_START("toTree");
    // Generate object structures for fast access
    levelRootNodes_.resize(level_data_.size());
    for (std::size_t i = 0; i < level_data_.size(); i++) {
        initAccessStructures(level_data_[i].save_objects, levelRootNodes_[i]);
    }

    initAccessStructures(save_objects_, rootNode_);
    TIME_MEASURE_END("toTree");

    // Count number of child objects in tree
    TIME_MEASURE_START("Count");
    for (auto& node : levelRootNodes_) {
        countObjects(node);
    }
    countObjects(rootNode_);
    TIME_MEASURE_END("Count");

    TIME_MEASURE_PRINT();
}

void Satisfactory3DMap::SaveGame::save(const std::filesystem::path& filepath) {
    // Serialize data to blob
    OMemStreamArchive ar(std::make_unique<MemOStream>());

    // Size placeholder
    ar.write<int32_t>(0);

    // Save levels
    ar.write(static_cast<int32_t>(level_data_.size()));

    for (auto& level : level_data_) {
        ar << level.level_name;
        saveTOCBlob(ar, level.save_objects, level.destroyed_actors_TOC);
        saveDataBlob(ar, level.save_objects);
        ar << level.destroyed_actors;
    }

    saveTOCBlob(ar, save_objects_, destroyed_actors_toc_);
    saveDataBlob(ar, save_objects_);

    // TODO
    ar.write<int32_t>(0);

    ar << unresolved_world_save_data_;

    // Store size
    uint64_t blob_size = ar.tell();
    ar.seek(0);
    ar.write(static_cast<int32_t>(blob_size) - 4);

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

        fileAr.write_vector(chunk_compressed);
    }
}

void Satisfactory3DMap::SaveGame::parseTOCBlob(IStreamArchive& ar, SaveObjectList& saveObjects,
    std::vector<ObjectReference>& destroyedActorsTOC) {
    const auto TOC_size = ar.read<int32_t>();
    const auto TOC_pos = ar.tell();

    // Parse objects
    TIME_MEASURE_START("ObjHead");
    const auto num_objects = ar.read<int32_t>();
    saveObjects.reserve(num_objects);
    for (int32_t i = 0; i < num_objects; i++) {
        saveObjects.emplace_back(SaveObjectBase::create(i, ar));
    }
    TIME_MEASURE_END("ObjHead");

    ar << destroyedActorsTOC;

    if (ar.tell() - TOC_pos != TOC_size) {
        throw std::runtime_error("Invalid size of TOCBlob!");
    }
}

void Satisfactory3DMap::SaveGame::parseDataBlob(IStreamArchive& ar, SaveObjectList& saveObjects) {
    const auto data_size = ar.read<int32_t>();
    const auto data_pos = ar.tell();

    // Parse object properties
    TIME_MEASURE_START("ObjProp");
    const auto num_object_data = ar.read<int32_t>();
    if (saveObjects.size() != static_cast<std::size_t>(num_object_data)) {
        throw std::runtime_error("Bad number of object data!");
    }
    // TODO: we can potentially do this in parallel, but this requires a thread pool and worker queue.
    for (int32_t i = 0; i < num_object_data; i++) {
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

void Satisfactory3DMap::SaveGame::initAccessStructures(const SaveObjectList& saveObjects, SaveNode& rootNode) {
    for (const auto& obj : saveObjects) {
        const auto& objName = obj->reference().pathName();

        // Store objects into map for access by name
        auto info = path_object_map_.emplace(objName, obj);
        if (!info.second) {
            throw std::runtime_error("Path name is not unique");
        }

        // Store objects into a tree structure for access by class
        std::reference_wrapper<SaveNode> n = rootNode;
        for (const auto& s : splitPathName(obj->className())) {
            n = n.get().childNodes[s];
        }
        if (n.get().objects.find(objName) != n.get().objects.end()) {
            throw std::runtime_error("Object name is not unique!");
        }
        n.get().objects[objName] = obj;
    }

    countObjects(rootNode);
}

void Satisfactory3DMap::SaveGame::saveTOCBlob(OStreamArchive& ar, SaveObjectList& saveObjects,
    std::vector<ObjectReference>& destroyedActorsTOC) {
    auto pos_size = ar.tell();
    ar.write<int32_t>(0);

    auto pos_before = ar.tell();

    // Write objects
    ar.write(static_cast<int32_t>(saveObjects.size()));
    for (const auto& obj : saveObjects) {
        ar << *obj;
    }

    ar << destroyedActorsTOC;

    auto pos_after = ar.tell();

    ar.seek(pos_size);
    ar.write(static_cast<int32_t>(pos_after - pos_before));
    ar.seek(pos_after);
}

void Satisfactory3DMap::SaveGame::saveDataBlob(OStreamArchive& ar, SaveObjectList& saveObjects) {
    auto blob_pos_size = ar.tell();
    ar.write<int32_t>(0);

    auto blob_pos_before = ar.tell();

    // Write object properties
    ar.write(static_cast<int32_t>(saveObjects.size()));
    for (const auto& obj : saveObjects) {
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
    ar.write(static_cast<int32_t>(blob_pos_after - blob_pos_before));
    ar.seek(blob_pos_after);
}
