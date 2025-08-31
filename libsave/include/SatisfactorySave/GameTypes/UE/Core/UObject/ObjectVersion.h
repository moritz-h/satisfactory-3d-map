#pragma once

#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FPackageFileVersion {
        int32_t FileVersionUE4 = 0;
        int32_t FileVersionUE5 = 0;

        void serialize(Archive& ar) {
            ar << FileVersionUE4;
            ar << FileVersionUE5;
        }
    };
} // namespace SatisfactorySave
