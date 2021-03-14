#include "SaveGame.h"

#include <fstream>
#include <functional>
#include <stdexcept>
#include <utility>

#include "Objects/SaveActor.h"
#include "Objects/SaveObject.h"
#include "Types/ChunkHeader.h"
#include "Utils/StreamUtils.h"
#include "Utils/StringUtils.h"
#include "Utils/ZlibUtils.h"

Satisfactory3DMap::SaveGame::SaveGame(const std::filesystem::path& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot read file!");
    }

    file.seekg(0, std::ios::end);
    auto filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    header_ = std::make_unique<SaveHeader>(file);

    auto file_data_blob = std::make_unique<std::vector<char>>();

    while (file.tellg() < filesize) {
        ChunkHeader chunk_header(file);
        auto chunk_compressed = read_vector<char>(file, chunk_header.compressedLength());
        std::vector<char> chunk_decompressed = zlibUncompress(chunk_compressed, chunk_header.decompressedLength());

        // Test equality by compressing again.
        // std::vector<char> test_buf = zlibCompress(chunk_decompressed);
        // if (chunk_compressed != test_buf) {
        //     throw std::runtime_error("Compression is not binary identical!");
        // }

        file_data_blob->insert(file_data_blob->end(), chunk_decompressed.begin(), chunk_decompressed.end());
    }

    auto file_data_blob_size = file_data_blob->size();

    MemIStream file_data_blob_stream(std::move(file_data_blob));

    if (static_cast<int32_t>(file_data_blob_size - sizeof(int32_t)) != read<int32_t>(file_data_blob_stream)) {
        throw std::runtime_error("Bad blob size!");
    }

    auto world_object_count = read<int32_t>(file_data_blob_stream);

    save_objects_.reserve(world_object_count);

    for (int i = 0; i < world_object_count; ++i) {
        auto type = read<int32_t>(file_data_blob_stream);
        switch (type) {
            case 0: { // object
                save_objects_.emplace_back(std::make_shared<SaveObject>(type, file_data_blob_stream));
                break;
            }
            case 1: { // actor
                save_objects_.emplace_back(std::make_shared<SaveActor>(type, file_data_blob_stream));
                break;
            }
            default: {
                throw std::runtime_error("Unknown object type!");
                break;
            }
        }
    }

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

    auto world_object_data_count = read<int32_t>(file_data_blob_stream);
    if (world_object_count != world_object_data_count) {
        throw std::runtime_error("Bad number of object data!");
    }

    for (int i = 0; i < world_object_data_count; i++) {
        // Check stream pos to validate parser.
        auto length = read<int32_t>(file_data_blob_stream);
        auto pos_before = file_data_blob_stream.tellg();
        save_objects_[i]->parseData(length, file_data_blob_stream);
        auto pos_after = file_data_blob_stream.tellg();
        if (pos_after - pos_before != length) {
            throw std::runtime_error("Error parsing object data!");
        }
    }

    auto collected_objects_count = read<int32_t>(file_data_blob_stream);
    collected_objects_.reserve(collected_objects_count);
    for (int i = 0; i < collected_objects_count; i++) {
        collected_objects_.emplace_back(file_data_blob_stream);
    }

    if (static_cast<long>(file_data_blob_size) != file_data_blob_stream.tellg()) {
        throw std::runtime_error("Error parsing save file: Size check after parsing failed!");
    }
}

void Satisfactory3DMap::SaveGame::printHeader() const {
    header_->print();
}
