#include "SaveGame.h"

#include <fstream>
#include <functional>
#include <stdexcept>
#include <utility>

#include "Objects/SaveActor.h"
#include "Objects/SaveObject.h"
#include "Utils/StreamUtils.h"
#include "Utils/StringUtils.h"
#include "Utils/TimeMeasure.h"
#include "Utils/ZlibUtils.h"

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
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot read file!");
    }

    // File size
    file.seekg(0, std::ios::end);
    const auto filesize = file.tellg();
    file.seekg(0, std::ios::beg);
    TIME_MEASURE_END("Open");

    // Read header
    TIME_MEASURE_START("Header");
    header_ = std::make_unique<SaveHeader>(file);
    TIME_MEASURE_END("Header");

    // Read and decompress chunks
    // Start with reading chunk headers and compressed buffers. Then the size of the decompressed blob is known in
    // advance and be allocated all at once. Decompression then can write directory to this final buffer without
    // any reallocation. Further the decompression of all chunks can run completely in parallel.
    TIME_MEASURE_START("Chunk");
    std::vector<ChunkInfo> chunk_list;
    std::size_t total_decompressed_size = 0;
    while (file.tellg() < filesize) {
        const ChunkHeader chunk_header(file);
        chunk_list.emplace_back(chunk_header, std::move(read_vector<char>(file, chunk_header.compressedLength())),
            total_decompressed_size);
        total_decompressed_size += chunk_header.decompressedLength();
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
        zlibUncompress(decompressed_buffer_ptr, chunk.header.decompressedLength(), chunk.compressed_chunk.data(),
            chunk.compressed_chunk.size());
    }
    TIME_MEASURE_END("Chunk");

    // Store size and init memory stream
    TIME_MEASURE_START("toStream");
    const auto file_data_blob_size = file_data_blob->size();
    MemIStream file_data_blob_stream(std::move(file_data_blob));

    // Validate blob size
    if (static_cast<int32_t>(file_data_blob_size - sizeof(int32_t)) != read<int32_t>(file_data_blob_stream)) {
        throw std::runtime_error("Bad blob size!");
    }
    TIME_MEASURE_END("toStream");

    // Parse objects
    TIME_MEASURE_START("ObjHead");
    const auto world_object_count = read<int32_t>(file_data_blob_stream);
    save_objects_.reserve(world_object_count);
    for (int32_t i = 0; i < world_object_count; ++i) {
        const auto type = read<int32_t>(file_data_blob_stream);
        switch (type) {
            case 0: { // object
                save_objects_.emplace_back(std::make_shared<SaveObject>(i, type, file_data_blob_stream));
                break;
            }
            case 1: { // actor
                save_objects_.emplace_back(std::make_shared<SaveActor>(i, type, file_data_blob_stream));
                break;
            }
            default: {
                throw std::runtime_error("Unknown object type!");
                break;
            }
        }
    }
    TIME_MEASURE_END("ObjHead");

    // Parse object properties
    TIME_MEASURE_START("ObjProp");
    const auto world_object_data_count = read<int32_t>(file_data_blob_stream);
    if (world_object_count != world_object_data_count) {
        throw std::runtime_error("Bad number of object data!");
    }
    // TODO: we can potentially do this in parallel, but this requires a thread pool and worker queue.
    for (int32_t i = 0; i < world_object_data_count; i++) {
        // Check stream pos to validate parser.
        const auto length = read<int32_t>(file_data_blob_stream);
        auto pos_before = file_data_blob_stream.tellg();
        save_objects_[i]->parseData(length, file_data_blob_stream);
        auto pos_after = file_data_blob_stream.tellg();
        if (pos_after - pos_before != length) {
            throw std::runtime_error("Error parsing object data!");
        }
    }
    TIME_MEASURE_END("ObjProp");

    // Parse collected objects
    TIME_MEASURE_START("Collect");
    const auto collected_objects_count = read<int32_t>(file_data_blob_stream);
    collected_objects_.reserve(collected_objects_count);
    for (int32_t i = 0; i < collected_objects_count; i++) {
        collected_objects_.emplace_back(file_data_blob_stream);
    }
    TIME_MEASURE_END("Collect");

    // Validate stream is completely read
    TIME_MEASURE_START("toTree");
    if (static_cast<long>(file_data_blob_size) != file_data_blob_stream.tellg()) {
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
