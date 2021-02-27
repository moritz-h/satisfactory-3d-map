#include "SaveGame.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "Types/SaveActor.h"
#include "Types/SaveObject.h"
#include "Utils/StreamUtils.h"
#include "Utils/ZlibUtils.h"

using namespace SatisfactorySaveGame;

SaveGame::SaveGame(const std::filesystem::path& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot read file!");
    }

    file.seekg(0, std::ios::end);
    auto filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    // See FGSaveSystem.h
    header_.save_header_version = read<int32_t>(file);
    header_.save_version = read<int32_t>(file);
    header_.build_version = read<int32_t>(file);
    header_.map_name = read_length_string(file);
    header_.map_options = read_length_string(file);
    header_.session_name = read_length_string(file);
    header_.play_duration = read<int32_t>(file);
    header_.save_date_time = read<int64_t>(file);
    header_.session_visibility = read<int8_t>(file);

    if (header_.save_header_version != 6 || header_.save_version != 25) {
        throw std::runtime_error("Unknown Save Version!");
    }

    auto file_data_blob = std::make_unique<std::vector<char>>();

    while (file.tellg() < filesize) {
        auto package_file_tag = read<uint64_t>(file);
        auto max_chunk_size = read<uint64_t>(file);
        auto compressed_length_1 = read<uint64_t>(file);
        auto decompressed_length_1 = read<uint64_t>(file);
        auto compressed_length_2 = read<uint64_t>(file);
        auto decompressed_length_2 = read<uint64_t>(file);

        if (package_file_tag != 2653586369) {
            throw std::runtime_error("Unknown package file tag!");
        }
        if (max_chunk_size != 131072) {
            throw std::runtime_error("Unknown max chunk size!");
        }
        if (compressed_length_1 != compressed_length_2) {
            throw std::runtime_error("Unknown chunk size!");
        }
        if (decompressed_length_1 != decompressed_length_2) {
            throw std::runtime_error("Unknown chunk size!");
        }
        if (decompressed_length_1 > max_chunk_size) {
            throw std::runtime_error("Chunk size larger than max chunk size!");
        }

        auto chunk_compressed = read_vector<char>(file, compressed_length_1);
        std::vector<char> chunk_decompressed = zlibUncompress(chunk_compressed, decompressed_length_1);

        // Test equality by compressing again.
        // std::vector<char> test_buf = zlibCompress(chunk_decompressed);
        // if (chunk_compressed != test_buf) {
        //     throw std::runtime_error("Compression is not binary identical!");
        // }

        file_data_blob->insert(file_data_blob->end(), chunk_decompressed.begin(), chunk_decompressed.end());
    }

    auto file_data_blob_size = file_data_blob->size();

    MemIStream file_data_blob_stream(std::move(file_data_blob));

    if (file_data_blob_size - sizeof(int32_t) != read<int32_t>(file_data_blob_stream)) {
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
    for (int i = 0; i < collected_objects_count; i++) {
        ObjectReference ref;
        ref.level_name = read_length_string(file_data_blob_stream);
        ref.path_name = read_length_string(file_data_blob_stream);
    }

    if (file_data_blob_size != file_data_blob_stream.tellg()) {
        throw std::runtime_error("Error parsing save file: Size check after parsing failed!");
    }
}

void SaveGame::printHeader() const {
    // save_date_time is integer ticks since 0001-01-01 00:00, where 1 tick is 100 nano seconds.
    // See: https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FDateTime/index.html
    // Satisfactory seems to use UTC.
    // Convert to unix timestamp:
    // Python: (datetime.datetime(1970, 1, 1) - datetime.datetime(1, 1, 1)).total_seconds()
    //   => 62135596800.0 seconds
    std::time_t save_date_time = (header_.save_date_time - 621355968000000000) / 10000000;

    // To string
    std::string save_date_str(20, '\0');
    std::strftime(save_date_str.data(), save_date_str.size(), "%F %T", std::localtime(&save_date_time));
    save_date_str.erase(std::find(save_date_str.begin(), save_date_str.end(), '\0'), save_date_str.end());

    // Print
    std::cout << "Save Header Version:     " << header_.save_header_version << std::endl;
    std::cout << "Save Version:            " << header_.save_version << std::endl;
    std::cout << "Build Version:           " << header_.build_version << std::endl;
    std::cout << "Map Name:                " << header_.map_name << std::endl;
    std::cout << "Map Options:             " << header_.map_options << std::endl;
    std::cout << "Session Name:            " << header_.session_name << std::endl;
    std::cout << "Play Duration (seconds): " << header_.play_duration << " (" << header_.play_duration / 60.0 / 60.0
              << " h)" << std::endl;
    std::cout << "Save Date Time (ticks):  " << header_.save_date_time << " (" << save_date_str << ")" << std::endl;
    std::cout << "Session Visibility:      " << static_cast<bool>(header_.session_visibility) << std::endl;
}
