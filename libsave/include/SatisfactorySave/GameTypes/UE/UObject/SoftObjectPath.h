#pragma once

#include <string>
#include <utility>

#include "../../../IO/Archive/Archive.h"
#include "Name.h"

namespace SatisfactorySave {

    // FSoftObjectPath
    class FSoftObjectPath {
    public:
        FSoftObjectPath() = default;
        FSoftObjectPath(FName AssetPathName, std::string SubPathString)
            : AssetPathName_(std::move(AssetPathName)),
              SubPathString_(std::move(SubPathString)){};

        void serialize(Archive& ar) {
            ar << AssetPathName_;
            ar << SubPathString_;
        }

        [[nodiscard]] const FName& AssetPathName() const {
            return AssetPathName_;
        }

        [[nodiscard]] const std::string& SubPathString() const {
            return SubPathString_;
        }

    private:
        FName AssetPathName_;
        std::string SubPathString_;
    };
} // namespace SatisfactorySave
