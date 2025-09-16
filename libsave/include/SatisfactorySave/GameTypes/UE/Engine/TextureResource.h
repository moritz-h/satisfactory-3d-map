#pragma once

#include <cstdint>

#include "../../../IO/Archive/Archive.h"
#include "../CoreUObject/Serialization/BulkData.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FTexture2DMipMap {
        FByteBulkData BulkData;
        int32_t SizeX = 0;
        int32_t SizeY = 0;
        int32_t SizeZ = 0;

        void serialize(Archive& ar) {
            // https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/Engine/Private/Texture2D.cpp#L146
            // assume bSerializeMipData == true
            ar << BulkData;
            ar << SizeX;
            ar << SizeY;
            ar << SizeZ;
        }
    };
} // namespace SatisfactorySave
