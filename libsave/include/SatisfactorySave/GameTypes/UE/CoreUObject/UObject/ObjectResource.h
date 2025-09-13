#pragma once

#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API FPackageIndex {
    public:
        int32_t Index;

        void serialize(Archive& ar) {
            ar << Index;
        }
    };
} // namespace SatisfactorySave
