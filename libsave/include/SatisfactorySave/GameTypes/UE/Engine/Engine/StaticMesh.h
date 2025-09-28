#pragma once

#include <cstdint>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../../../FactoryGame/FGObjectReference.h"
#include "../../Core/UObject/NameTypes.h"
#include "../../CoreUObject/UObject/Object.h"
#include "../MeshUVChannelInfo.h"
#include "../StaticMeshResources.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FStaticMaterial {
        FObjectReferenceDisc MaterialInterface;
        FName MaterialSlotName;
        FMeshUVChannelInfo UVChannelData;

        void serialize(Archive& ar) {
            ar << MaterialInterface;
            ar << MaterialSlotName;
            ar << UVChannelData;
        }
    };

    class SATISFACTORYSAVE_API UStaticMesh : public UObject {
    public:
        FStaticMeshRenderData RenderData;
        std::vector<FStaticMaterial> StaticMaterials;
        uint8_t MaxShadowLOD = 0;

        void serialize(Archive& ar) override;
    };
} // namespace SatisfactorySave
