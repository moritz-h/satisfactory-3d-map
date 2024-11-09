#pragma once

#include "../../../../IO/Archive/Archive.h"
#include "../../Core/UObject/NameTypes.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FTopLevelAssetPath {
    public:
        FName PackageName;
        FName AssetName;

        FTopLevelAssetPath() = default;

        void serialize(Archive& ar) {
            ar << PackageName;
            ar << AssetName;
        }
    };
} // namespace SatisfactorySave
