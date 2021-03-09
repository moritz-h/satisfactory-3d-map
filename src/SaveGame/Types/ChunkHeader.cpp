#include "ChunkHeader.h"

#include "../Utils/StreamUtils.h"

Satisfactory3DMap::ChunkHeader::ChunkHeader(std::istream& stream) {
    package_file_tag_ = read<uint64_t>(stream);
    max_chunk_size_ = read<uint64_t>(stream);
    compressed_length_1_ = read<uint64_t>(stream);
    decompressed_length_1_ = read<uint64_t>(stream);
    compressed_length_2_ = read<uint64_t>(stream);
    decompressed_length_2_ = read<uint64_t>(stream);
    if (package_file_tag_ != 2653586369) {
        throw std::runtime_error("Unknown package file tag!");
    }
    if (max_chunk_size_ != 131072) {
        throw std::runtime_error("Unknown max chunk size!");
    }
    if (compressed_length_1_ != compressed_length_2_) {
        throw std::runtime_error("Unknown chunk size!");
    }
    if (decompressed_length_1_ != decompressed_length_2_) {
        throw std::runtime_error("Unknown chunk size!");
    }
    if (decompressed_length_1_ > max_chunk_size_) {
        throw std::runtime_error("Chunk size larger than max chunk size!");
    }
}
