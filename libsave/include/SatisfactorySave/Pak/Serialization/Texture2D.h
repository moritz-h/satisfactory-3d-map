#pragma once

#include <optional>
#include <string>
#include <vector>

#include "../../GameTypes/Properties/Base/PropertyList.h"
#include "../../GameTypes/UE/Core/Misc/Guid.h"
#include "../../GameTypes/UE/CoreUObject/Serialization/BulkData.h"
#include "../../IO/Archive/Archive.h"
#include "../AssetFile.h"
#include "StripDataFlags.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FTexture2DMipMap {
        FByteBulkData BulkData;
        int32_t SizeX = 0;
        int32_t SizeY = 0;
        int32_t SizeZ = 0;

        // FTexture2DMipMap::Serialize
        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/Texture2D.cpp#L125
        void serialize(Archive& ar) {
            ar << BulkData;
            ar << SizeX;
            ar << SizeY;
            ar << SizeZ;
        }
    };

    struct SATISFACTORYSAVE_API FTexturePlatformData {
        int32_t SizeX = 0;
        int32_t SizeY = 0;
        uint32_t PackedData = 0;
        std::string PixelFormatString;
        int32_t FirstMipToSerialize = 0;
        std::vector<FTexture2DMipMap> Mips;
        bool bIsVirtual = false;

        // FTexturePlatformData::SerializeCooked
        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/TextureDerivedData.cpp#L1562
        void serialize(Archive& ar) {

            char bUsingDerivedData = 0; // bool
            ar << bUsingDerivedData;

            if (bUsingDerivedData) {
                throw std::runtime_error("bUsingDerivedData == true not implemented!");
            }

            std::vector<uint8_t> PlaceholderDerivedData(15);
            ar.serializeRaw(PlaceholderDerivedData.data(), 15);

            ar << SizeX;
            ar << SizeY;
            ar << PackedData;
            ar << PixelFormatString;
            ar << FirstMipToSerialize;

            int32_t NumMips = 0;
            ar << NumMips;
            Mips.resize(NumMips);
            for (auto& mip : Mips) {
                ar << mip;
            }

            ar << bIsVirtual;

            if (bIsVirtual) {
                throw std::runtime_error("bIsVirtual == true not implemented!");
            }
        }
    };

    // UTexture2D
    class SATISFACTORYSAVE_API Texture2D {
    public:
        Texture2D() = default;

        void serialize(Archive& ar);

        [[nodiscard]] const FTexturePlatformData& runningPlatformData() const {
            return RunningPlatformData;
        }

    protected:
        PropertyList properties_;
        std::optional<FGuid> guid_;

        FTexturePlatformData RunningPlatformData;
    };
} // namespace SatisfactorySave
