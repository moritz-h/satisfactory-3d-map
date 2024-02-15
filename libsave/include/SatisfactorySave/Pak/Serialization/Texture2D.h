#pragma once

#include "../../GameTypes/Properties/Base/PropertyList.h"
#include "../../GameTypes/UE/Misc/Guid.h"
#include "../../IO/Archive/Archive.h"
#include "../AssetFile.h"
#include "StripDataFlags.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FByteBulkData {
        uint32_t BulkDataFlags = 0; // EBulkDataFlags
        int32_t ElementCount = 0;
        int32_t BulkDataSizeOnDisk = 0;
        int64_t BulkDataOffsetInFile = 0;
        std::vector<char> data;

        // FUntypedBulkData::Serialize
        void serialize(Archive& ar) {
            ar << BulkDataFlags;
            ar << ElementCount;
            ar << BulkDataSizeOnDisk;
            ar << BulkDataOffsetInFile;

            if (BulkDataFlags == 0x48) {
                // inline data
                if (BulkDataOffsetInFile != static_cast<int64_t>(ar.tell())) {
                    throw std::runtime_error("Unexpected file format!");
                }
                auto& inAr = dynamic_cast<IStreamArchive&>(ar);
                data = inAr.read_buffer(BulkDataSizeOnDisk);
            } else if (BulkDataFlags == 0x010501) {
                // bulk data
                auto& inAr = dynamic_cast<AssetFile&>(ar);
                const auto pos_before = ar.tell();
                ar.seek(BulkDataOffsetInFile + inAr.summary().BulkDataStartOffset);
                data = inAr.read_buffer(BulkDataSizeOnDisk);
                ar.seek(pos_before);
            } else {
                throw std::runtime_error("BulkDataFlags not implemented!");
            }
        }
    };

    struct SATISFACTORYSAVE_API FTexture2DMipMap {
        bool bCooked = false;
        FByteBulkData BulkData;
        int32_t SizeX = 0;
        int32_t SizeY = 0;
        int32_t SizeZ = 0;

        // FTexture2DMipMap::Serialize
        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/Texture2D.cpp#L125
        void serialize(Archive& ar) {
            ar << bCooked;
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
        bool hasGuid_ = false;
        FGuid guid_;

        FTexturePlatformData RunningPlatformData;
    };
} // namespace SatisfactorySave
