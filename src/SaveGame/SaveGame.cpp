#include "SaveGame.h"

#include <fstream>
#include <functional>
#include <stdexcept>
#include <utility>

#include "Objects/SaveActor.h"
#include "Objects/SaveObject.h"
#include "Utils/StreamUtils.h"
#include "Utils/StringUtils.h"
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
    // Open file
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot read file!");
    }

    // File size
    file.seekg(0, std::ios::end);
    auto filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read header
    header_ = std::make_unique<SaveHeader>(file);

    // Read and decompress chunks
    // Start with reading chunk headers and compressed buffers. Then the size of the decompressed blob is known in
    // advance and be allocated all at once. Decompression then can write directory to this final buffer without
    // any reallocation. Further the decompression of all chunks can run completely in parallel.
    std::vector<ChunkInfo> chunk_list;
    std::size_t total_decompressed_size = 0;
    while (file.tellg() < filesize) {
        ChunkHeader chunk_header(file);
        chunk_list.emplace_back(
            chunk_header, std::move(read_vector<char>(file, chunk_header.compressedLength())), total_decompressed_size);
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

    // Store size and init memory stream
    auto file_data_blob_size = file_data_blob->size();
    MemIStream file_data_blob_stream(std::move(file_data_blob));

    // Validate blob size
    if (static_cast<int32_t>(file_data_blob_size - sizeof(int32_t)) != read<int32_t>(file_data_blob_stream)) {
        throw std::runtime_error("Bad blob size!");
    }

    // Parse objects
    auto world_object_count = read<int32_t>(file_data_blob_stream);
    save_objects_.reserve(world_object_count);
    for (int32_t i = 0; i < world_object_count; ++i) {
        auto type = read<int32_t>(file_data_blob_stream);
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

    // Parse object properties
    auto world_object_data_count = read<int32_t>(file_data_blob_stream);
    if (world_object_count != world_object_data_count) {
        throw std::runtime_error("Bad number of object data!");
    }
    for (int32_t i = 0; i < world_object_data_count; i++) {
        // Check stream pos to validate parser.
        auto length = read<int32_t>(file_data_blob_stream);
        auto pos_before = file_data_blob_stream.tellg();
        save_objects_[i]->parseData(length, file_data_blob_stream);
        auto pos_after = file_data_blob_stream.tellg();
        if (pos_after - pos_before != length) {
            throw std::runtime_error("Error parsing object data!");
        }
    }

    // Parse collected objects
    auto collected_objects_count = read<int32_t>(file_data_blob_stream);
    collected_objects_.reserve(collected_objects_count);
    for (int32_t i = 0; i < collected_objects_count; i++) {
        collected_objects_.emplace_back(file_data_blob_stream);
    }

    // Validate stream is completely read
    if (static_cast<long>(file_data_blob_size) != file_data_blob_stream.tellg()) {
        throw std::runtime_error("Error parsing save file: Size check after parsing failed!");
    }

    // Read objects into a tree structure for access by name
    for (const auto& obj : save_objects_) {
        std::reference_wrapper<SaveNode> n = rootNode_;
        for (const auto& s : splitPathName(obj->className())) {
            n = n.get().childNodes[s];
        }
        const auto& objName = obj->reference().pathName();
        if (n.get().objects.find(objName) != n.get().objects.end()) {
            throw std::runtime_error("Object name is not unique!");
        }
        n.get().objects[objName] = obj;
    }

    // Count number of child objects in tree
    countObjects(rootNode_);
}

void Satisfactory3DMap::SaveGame::printHeader() const {
    header_->print();
}
