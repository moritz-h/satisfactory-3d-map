#pragma once

#include <cstdint>

#include "IO/Archive/Archive.h"

namespace SatisfactorySave {

    /**
     * ChunkHeader
     *
     * Game uses FArchiveLoadCompressedProxy to read compressed chunks.
     * https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Serialization/ArchiveLoadCompressedProxy.h
     * Seems that there are stored multiple individual chunks which are serialized here:
     * https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Private/Serialization/Archive.cpp#L679
     *
     * Therefore, we seem to have 3 pairs of FCompressedChunkInfo as chunk header, where the first one is
     * PACKAGE_FILE_TAG and LOADING_COMPRESSION_CHUNK_SIZE, the second one the summary size, and the last one the chunk
     * size. As there is only one chunk at a time summary and chunk size are always the same.
     */
    class ChunkHeader {
    public:
        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/UObject/ObjectVersion.h#L9
        static constexpr uint32_t PACKAGE_FILE_TAG = 0x9E2A83C1;
        static constexpr uint32_t ARCHIVE_V2_HEADER = 0x22222222;

        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Core/Public/Misc/Compression.h#L22
        static constexpr int64_t COMPRESSION_CHUNK_SIZE = 131072;

        ChunkHeader() = default;
        ChunkHeader(int64_t compressedSize, int64_t uncompressedSize);

        void serialize(Archive& ar);

        [[nodiscard]] int64_t compressedSize() const {
            return compressed_size_;
        }

        [[nodiscard]] int64_t uncompressedSize() const {
            return uncompressed_size_;
        }

    protected:
        uint32_t package_file_tag_ = PACKAGE_FILE_TAG;
        uint32_t archive_header_ = ARCHIVE_V2_HEADER;
        int64_t compression_chunk_size_ = COMPRESSION_CHUNK_SIZE;
        uint8_t compressorNum_ = 3; // Zlib
        int64_t compressed_size_summary_ = 0;
        int64_t uncompressed_size_summary_ = 0;
        int64_t compressed_size_ = 0;
        int64_t uncompressed_size_ = 0;
    };
} // namespace SatisfactorySave
