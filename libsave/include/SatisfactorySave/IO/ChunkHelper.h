#pragma once

#include <memory>
#include <vector>

#include "GameTypes/Save/ChunkHeader.h"
#include "IO/Archive/IStreamArchive.h"

namespace Satisfactory3DMap {
    struct ChunkInfo {
        ChunkInfo(ChunkHeader header, std::vector<char> compressed_chunk, std::size_t decompressed_offset)
            : header(header),
              compressed_chunk(std::move(compressed_chunk)),
              decompressed_offset(decompressed_offset){};
        ChunkHeader header;
        std::vector<char> compressed_chunk;
        std::size_t decompressed_offset;
    };

    std::unique_ptr<std::vector<char>> decompressChunks(IFStreamArchive& fileAr);
} // namespace Satisfactory3DMap
