#pragma once

#include "../UE/Core/Math/Color.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FFactoryCustomizationColorSlot {
    public:
        FLinearColor PrimaryColor;
        FLinearColor SecondaryColor;
        FObjectReferenceDisc PaintFinish;
    };

    struct SATISFACTORYSAVE_API FFactoryCustomizationData {
    public:
        FObjectReferenceDisc SwatchDesc;
        FObjectReferenceDisc PatternDesc;
        FObjectReferenceDisc MaterialDesc;
        FObjectReferenceDisc SkinDesc;
        FFactoryCustomizationColorSlot OverrideColorData;
        uint8_t PatternRotation = 0;
    };

} // namespace SatisfactorySave
