#include "ChunkHeader.h"

Satisfactory3DMap::ChunkHeader::ChunkHeader(int64_t compressedSize, int64_t uncompressedSize)
    : package_file_tag_(PACKAGE_FILE_TAG),
      compression_chunk_size_(COMPRESSION_CHUNK_SIZE),
      compressed_size_summary_(compressedSize),
      uncompressed_size_summary_(uncompressedSize),
      compressed_size_(compressedSize),
      uncompressed_size_(uncompressedSize) {}

void Satisfactory3DMap::ChunkHeader::serialize(Archive& ar) {
    ar << package_file_tag_;
    ar << compression_chunk_size_;
    ar << compressed_size_summary_;
    ar << uncompressed_size_summary_;
    ar << compressed_size_;
    ar << uncompressed_size_;
    if (package_file_tag_ != PACKAGE_FILE_TAG) {
        throw std::runtime_error("Invalid package file tag!");
    }
    if (compression_chunk_size_ != 131072) {
        throw std::runtime_error("Invalid compression chunk size!");
    }
    if (compressed_size_summary_ != compressed_size_) {
        throw std::runtime_error("Compressed size does not match!");
    }
    if (uncompressed_size_summary_ != uncompressed_size_) {
        throw std::runtime_error("Uncompressed size does not match!");
    }
    if (uncompressed_size_ > compression_chunk_size_) {
        throw std::runtime_error("Chunk size larger than compression chunk size!");
    }
}
