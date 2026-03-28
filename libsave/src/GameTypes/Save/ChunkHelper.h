#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "ChunkHeader.h"
#include "IO/Archive/IStreamArchive.h"

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
} // namespace SatisfactorySave
