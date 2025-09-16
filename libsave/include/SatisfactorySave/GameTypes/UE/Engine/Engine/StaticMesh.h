#pragma once

#include "../../../../IO/Archive/Archive.h"
#include "../../CoreUObject/UObject/Object.h"
#include "../StaticMeshResources.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API UStaticMesh : public UObject {
    public:
        FStaticMeshRenderData RenderData;

        void serialize(Archive& ar) override;
    };
} // namespace SatisfactorySave
