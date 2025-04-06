#pragma once

#include "../../IO/Archive/Archive.h"
#include "../UE/Core/Containers/Map.h"
#include "../UE/Core/Math/Transform.h"
#include "../UE/Engine/GameFramework/Actor.h"
#include "FGDynamicStruct.h"
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
        FFGDynamicStruct TypeSpecificData;

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
            if (ar.getLightweightVersion() >= 2) {
                ar << TypeSpecificData;
            }
        }
    };

    class SATISFACTORYSAVE_API AFGLightweightBuildableSubsystem : public AActor {
    public:
        int32_t currentLightweightVersion = 2; // TODO use enum with latest version
        TMap<FObjectReferenceDisc, std::vector<FRuntimeBuildableInstanceData>> mBuildableClassToInstanceArray;

        void serialize(Archive& ar) override {
            AActor::serialize(ar);
            if (ar.getSaveVersion() >= 48) {
                ar << currentLightweightVersion;
            } else {
                currentLightweightVersion = 1;
            }
            auto lightweight_version_stack_pusher = ar.pushLightweightVersion(currentLightweightVersion);
            ar << mBuildableClassToInstanceArray;
        }
    };

} // namespace SatisfactorySave
