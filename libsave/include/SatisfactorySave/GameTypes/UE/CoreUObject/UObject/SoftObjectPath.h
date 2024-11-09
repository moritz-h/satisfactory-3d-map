#pragma once

#include <string>

#include "../../../../IO/Archive/Archive.h"
#include "TopLevelAssetPath.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FSoftObjectPath {
    public:
        FTopLevelAssetPath AssetPath;
        std::string SubPathString;

        FSoftObjectPath() = default;

        void serialize(Archive& ar) {
            ar << AssetPath;
            ar << SubPathString;
        }
    };
} // namespace SatisfactorySave
