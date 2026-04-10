#pragma once

#include <cstddef>
#include <memory>
#include <span>
#include <vector>

#include "ChunkHeader.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

namespace SatisfactorySave {
    struct ChunkInfo {
        ChunkInfo(ChunkHeader header, std::vector<std::byte> compressed_chunk, std::size_t decompressed_offset)
            : header(std::move(header)),
              compressed_chunk(std::move(compressed_chunk)),
              decompressed_offset(decompressed_offset) {}
        ChunkHeader header;
        std::vector<std::byte> compressed_chunk;
        std::size_t decompressed_offset;
    };

    std::vector<std::byte> decompressChunks(IStreamArchive& inAr);

    void compressChunks(OStreamArchive& outAr, std::span<const std::byte> blob);
} // namespace SatisfactorySave
