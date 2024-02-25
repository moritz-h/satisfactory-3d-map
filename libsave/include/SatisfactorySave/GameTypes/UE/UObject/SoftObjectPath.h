#pragma once

#include <string>
#include <utility>

#include "../../../IO/Archive/Archive.h"
#include "Name.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // FSoftObjectPath
    class SATISFACTORYSAVE_API FSoftObjectPath {
    public:
        FName AssetPathName;
        std::string SubPathString;

        FSoftObjectPath() = default;
        FSoftObjectPath(FName AssetPathName, std::string SubPathString)
            : AssetPathName(std::move(AssetPathName)),
              SubPathString(std::move(SubPathString)){};

        void serialize(Archive& ar) {
            ar << AssetPathName;
            ar << SubPathString;
        }
    };
} // namespace SatisfactorySave
