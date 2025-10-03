#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../../Core/Math/Color.h"
#include "../../Core/Misc/SecureHash.h"
#include "satisfactorysave_export.h"

#define VIRTUALTEXTURE_DATA_MAXLAYERS 8u

namespace SatisfactorySave {
    struct FVirtualTextureDataChunk {
        FByteBulkData BulkData;
        FSHAHash BulkDataHash;
        uint32_t SizeInBytes = 0;
        uint32_t CodecPayloadSize = 0;
        std::array<uint32_t, VIRTUALTEXTURE_DATA_MAXLAYERS> CodecPayloadOffset;
        std::array</*EVirtualTextureCodec*/ uint8_t, VIRTUALTEXTURE_DATA_MAXLAYERS> CodecType;
    };

    struct SATISFACTORYSAVE_API FVirtualTextureTileOffsetData {
        uint32_t Width = 0;
        uint32_t Height = 0;
        uint32_t MaxAddress = 0;
        std::vector<uint32_t> Addresses;
        std::vector<uint32_t> Offsets;

        void serialize(Archive& ar) {
            ar << Width;
            ar << Height;
            ar << MaxAddress;
            ar << Addresses;
            ar << Offsets;
        }
    };

    struct SATISFACTORYSAVE_API FVirtualTextureBuiltData {
        uint32_t NumLayers = 0;
        uint32_t WidthInBlocks = 0;
        uint32_t HeightInBlocks = 0;
        uint32_t TileSize = 0;
        uint32_t TileBorderSize = 0;
        std::vector<uint32_t> TileDataOffsetPerLayer;
        uint32_t NumMips = 0;
        uint32_t Width = 0;
        uint32_t Height = 0;
        std::vector<uint32_t> ChunkIndexPerMip;
        std::vector<uint32_t> BaseOffsetPerMip;
        std::vector<FVirtualTextureTileOffsetData> TileOffsetData;
        std::vector<uint32_t> TileIndexPerChunk;
        std::vector<uint32_t> TileIndexPerMip;
        std::vector<uint32_t> TileOffsetInChunk;
        std::array<FLinearColor, VIRTUALTEXTURE_DATA_MAXLAYERS> LayerFallbackColors;
        std::vector<FVirtualTextureDataChunk> Chunks;

        void serialize(Archive& ar) {
            bool bCooked = true;
            ar << bCooked;

            ar << NumLayers;
            ar << WidthInBlocks;
            ar << HeightInBlocks;
            ar << TileSize;
            ar << TileBorderSize;
            ar << TileDataOffsetPerLayer;

            ar << NumMips;
            ar << Width;
            ar << Height;
            ar << ChunkIndexPerMip;
            ar << BaseOffsetPerMip;
            ar << TileOffsetData;
            ar << TileIndexPerChunk;
            ar << TileIndexPerMip;
            ar << TileOffsetInChunk;

            // LayerTypes
            for (uint32_t Layer = 0; Layer < NumLayers; Layer++) {
                std::string PixelFormatString;
                ar << PixelFormatString;
            }

            for (uint32_t Layer = 0; Layer < NumLayers; Layer++) {
                ar << LayerFallbackColors[Layer];
            }

            int32_t NumChunksToSerialize = static_cast<int32_t>(Chunks.size());
            ar << NumChunksToSerialize;
            if (ar.isIArchive()) {
                Chunks.resize(NumChunksToSerialize);
            }

            for (auto& Chunk : Chunks) {
                ar << Chunk.BulkDataHash;
                ar << Chunk.SizeInBytes;
                ar << Chunk.CodecPayloadSize;
                for (uint32_t LayerIndex = 0; LayerIndex < NumLayers; ++LayerIndex) {
                    ar << Chunk.CodecType[LayerIndex];
                    ar << Chunk.CodecPayloadOffset[LayerIndex];
                }
                ar << Chunk.BulkData;
            }
        }
    };
} // namespace SatisfactorySave
