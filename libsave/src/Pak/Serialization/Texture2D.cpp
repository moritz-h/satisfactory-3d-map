#include "Pak/Serialization/Texture2D.h"

void SatisfactorySave::Texture2D::serialize(Archive& ar) {

    // From UObject::Serialize

    ar << properties_;
    ar << guid_;

    // UTexture::Serialize
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/Texture.cpp#L366

    FStripDataFlags flags;
    ar << flags;
    flags.validateOnlyEditorDataIsStripped();

    // UTexture2D::Serialize
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/Texture2D.cpp#L180

    ar << flags;
    flags.validateOnlyEditorDataIsStripped();

    bool bCooked = false;
    ar << bCooked;

    // UTexture::SerializeCookedPlatformData
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/Engine/Private/TextureDerivedData.cpp#L2043

    FName PixelFormatName;
    ar << PixelFormatName;

    while (PixelFormatName != "None") {
        int64_t SkipOffset = 0;
        ar << SkipOffset;

        ar << RunningPlatformData;

        ar << PixelFormatName;

        // Check that second entry is none, for now expect only one entry.
        if (PixelFormatName != "None") {
            throw std::runtime_error("more than one entry not implemented!");
        }
    }
}
