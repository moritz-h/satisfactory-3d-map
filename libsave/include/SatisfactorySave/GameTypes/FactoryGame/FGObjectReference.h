#pragma once

#include <string>
#include <utility>

#include "../../IO/Archive/Archive.h"
#include "../../Pak/AssetFile.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FObjectReferenceDisc {
    public:
        std::string LevelName;
        std::string PathName;

        FObjectReferenceDisc() = default;
        FObjectReferenceDisc(std::string levelName, std::string pathName)
            : LevelName(std::move(levelName)),
              PathName(std::move(pathName)) {}

        void serialize(Archive& ar) {
            ar << LevelName;
            ar << PathName;
        }

        // TODO hack for pak file, replace by clean solution for savegame and pak file
        [[nodiscard]] int32_t pakValue() const {
            return pak_value_;
        }

    private:
        friend class AssetFile;
        int32_t pak_value_ = 0;
    };

    inline bool operator==(const FObjectReferenceDisc& a, const FObjectReferenceDisc& b) {
        return a.LevelName == b.LevelName && a.PathName == b.PathName;
    }
} // namespace SatisfactorySave
