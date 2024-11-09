#pragma once

#include "../../../IO/Archive/Archive.h"
#include "../Containers/Map.h"
#include "../GameFramework/Actor.h"
#include "../Math/Color.h"
#include "../Math/Transform.h"
#include "ObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // From FGFactoryColoringTypes.h
    struct SATISFACTORYSAVE_API FFactoryCustomizationColorSlot {
    public:
        FLinearColor PrimaryColor;
        FLinearColor SecondaryColor;
        FObjectReferenceDisc PaintFinish;
    };

    // From FGFactoryColoringTypes.h
    struct SATISFACTORYSAVE_API FFactoryCustomizationData {
    public:
        FObjectReferenceDisc SwatchDesc;
        FObjectReferenceDisc PatternDesc;
        FObjectReferenceDisc MaterialDesc;
        FObjectReferenceDisc SkinDesc;
        FFactoryCustomizationColorSlot OverrideColorData;
        uint8_t PatternRotation = 0;
    };

    struct SATISFACTORYSAVE_API FRuntimeBuildableInstanceData {
    public:
        FTransform Transform;
        FFactoryCustomizationData CustomizationData;
        FObjectReferenceDisc BuiltWithRecipe;
        FObjectReferenceDisc BlueprintProxy;

        void serialize(Archive& ar) {
            ar << Transform;
            ar << CustomizationData.SwatchDesc;
            ar << CustomizationData.MaterialDesc;
            ar << CustomizationData.PatternDesc;
            ar << CustomizationData.SkinDesc;
            ar << CustomizationData.OverrideColorData.PrimaryColor;
            ar << CustomizationData.OverrideColorData.SecondaryColor;
            ar << CustomizationData.OverrideColorData.PaintFinish;
            ar << CustomizationData.PatternRotation;
            ar << BuiltWithRecipe;
            ar << BlueprintProxy;
        }
    };

    class SATISFACTORYSAVE_API AFGLightweightBuildableSubsystem : public AActor {
    public:
        TMap<FObjectReferenceDisc, std::vector<FRuntimeBuildableInstanceData>> mBuildableClassToInstanceArray;

        void serialize(Archive& ar) override {
            AActor::serialize(ar);
            ar << mBuildableClassToInstanceArray;
        }
    };

} // namespace SatisfactorySave
