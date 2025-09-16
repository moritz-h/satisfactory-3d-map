#pragma once

#include "../../../../IO/Archive/Archive.h"
#include "../../CoreUObject/UObject/Object.h"
#include "Texture.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API UTexture2D : public UObject {
    public:
        FTexturePlatformData RunningPlatformData;

        void serialize(Archive& ar) override;
    };
} // namespace SatisfactorySave
