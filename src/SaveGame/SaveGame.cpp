#include "SaveGame.h"

#include <fstream>
#include <functional>
#include <stdexcept>
#include <utility>

#include "GameTypes/SaveObjects/SaveActor.h"
#include "GameTypes/SaveObjects/SaveObject.h"
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

    // Parse objects
    TIME_MEASURE_START("ObjHead");
    const auto world_object_count = ar.read<int32_t>();
    save_objects_.reserve(world_object_count);
    for (int32_t i = 0; i < world_object_count; ++i) {
        save_objects_.emplace_back(SaveObjectBase::create(i, ar));
    }
    TIME_MEASURE_END("ObjHead");

    // Parse object properties
    TIME_MEASURE_START("ObjProp");
    const auto world_object_data_count = ar.read<int32_t>();
    if (world_object_count != world_object_data_count) {
        throw std::runtime_error("Bad number of object data!");
    }
    // TODO: we can potentially do this in parallel, but this requires a thread pool and worker queue.
    for (int32_t i = 0; i < world_object_data_count; i++) {
        // Check stream pos to validate parser.
        const auto length = ar.read<int32_t>();
        auto pos_before = ar.tell();
        save_objects_[i]->serializeProperties(ar, length);
        auto pos_after = ar.tell();
        if (pos_after - pos_before != length) {
            throw std::runtime_error("Error parsing object data!");
        }
    }
    TIME_MEASURE_END("ObjProp");

    // Parse collected objects
    TIME_MEASURE_START("Collect");
    const auto collected_objects_count = ar.read<int32_t>();
    collected_objects_.reserve(collected_objects_count);
    for (int32_t i = 0; i < collected_objects_count; i++) {
        ObjectReference ref;
        ar << ref;
        collected_objects_.push_back(std::move(ref));
    }
    TIME_MEASURE_END("Collect");

    // Validate stream is completely read
    TIME_MEASURE_START("toTree");
    if (static_cast<long>(file_data_blob_size) != ar.tell()) {
        throw std::runtime_error("Error parsing save file: Size check after parsing failed!");
    }

    // Generate object structures for fast access
    for (const auto& obj : save_objects_) {
        const auto& objName = obj->reference().pathName();

        // Store objects into map for access by name
        auto info = path_object_map_.emplace(objName, obj);
        if (!info.second) {
            throw std::runtime_error("Path name is not unique");
        }

        // Store objects into a tree structure for access by class
        std::reference_wrapper<SaveNode> n = rootNode_;
        for (const auto& s : splitPathName(obj->className())) {
            n = n.get().childNodes[s];
        }
        if (n.get().objects.find(objName) != n.get().objects.end()) {
            throw std::runtime_error("Object name is not unique!");
        }
        n.get().objects[objName] = obj;
    }
    TIME_MEASURE_END("toTree");

    // Count number of child objects in tree
    TIME_MEASURE_START("Count");
    countObjects(rootNode_);
    TIME_MEASURE_END("Count");

    TIME_MEASURE_PRINT();
}

void Satisfactory3DMap::SaveGame::save(const std::filesystem::path& filepath) {
    // Serialize data to blob
    OMemStreamArchive ar(std::make_unique<MemOStream>());

    // Size placeholder
    ar.write<int32_t>(0);

    // Write objects
    ar.write(static_cast<int32_t>(save_objects_.size()));
    for (const auto& obj : save_objects_) {
        ar << *obj;
    }

    // Write object properties
    ar.write(static_cast<int32_t>(save_objects_.size()));
    for (const auto& obj : save_objects_) {
        auto pos_length = ar.tell();
        ar.write<int32_t>(0);

        auto pos_before = ar.tell();
        obj->serializeProperties(ar, 0);
        auto pos_after = ar.tell();

        ar.seek(pos_length);
        ar.write(static_cast<int32_t>(pos_after - pos_before));
        ar.seek(pos_after);
    }

    // Write collected objects
    ar.write(static_cast<int32_t>(collected_objects_.size()));
    for (auto& obj : collected_objects_) {
        ar << obj;
    }

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
