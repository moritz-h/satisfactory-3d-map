#pragma once

#include "../../IO/Archive/Archive.h"
#include "../UE/Core/Containers/Map.h"
#include "../UE/Core/Math/Transform.h"
#include "../UE/Engine/GameFramework/Actor.h"
#include "FGFactoryColoringTypes.h"
#include "FGObjectReference.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

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
