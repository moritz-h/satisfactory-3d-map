#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../TextureResource.h"
#include "../VT/VirtualTextureBuiltData.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FTexturePlatformData {
        int32_t SizeX = 0;
        int32_t SizeY = 0;
        uint32_t PackedData = 0;
        std::string PixelFormatString;
        int32_t FirstMipToSerialize = 0;
        std::vector<FTexture2DMipMap> Mips;
        bool bIsVirtual = false;
        FVirtualTextureBuiltData VTData;

        void serialize(Archive& ar);
    };
} // namespace SatisfactorySave
