#include "GameTypes/UE/Engine/Engine/Texture.h"

void SatisfactorySave::FTexturePlatformData::serialize(Archive& ar) {
    if (!ar.isIArchive()) {
        throw std::runtime_error("Only IStreamArchive support implemented!");
    }

    // FTexturePlatformData::SerializeCooked
    // https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/Engine/Private/TextureDerivedData.cpp#L2907

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
        ar << VTData;
    }
}
