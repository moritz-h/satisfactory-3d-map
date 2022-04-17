#pragma once

#include <string>
#include <utility>

#include "IO/Archive/Archive.h"
#include "IO/Pak/AssetFile.h"

namespace Satisfactory3DMap {

    // FObjectReferenceDisc
    class ObjectReference {
    public:
        ObjectReference() = default;
        ObjectReference(std::string level_name, std::string path_name)
            : level_name_(std::move(level_name)),
              path_name_(std::move(path_name)){};

        void serialize(Archive& ar) {
            ar << level_name_;
            ar << path_name_;
        }

        [[nodiscard]] const std::string& levelName() const {
            return level_name_;
        }

        [[nodiscard]] const std::string& pathName() const {
            return path_name_;
        }

        // TODO hack for pack file, replace by clean solution for savegame and pakfile"
        [[nodiscard]] int32_t pakValue() const {
            return pak_value_;
        }

    private:
        std::string level_name_;
        std::string path_name_;

        friend class AssetFile;
        int32_t pak_value_;
    };
} // namespace Satisfactory3DMap
