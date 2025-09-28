#pragma once

#include "../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FPerPlatformFloat {
    public:
        bool bCooked = false;
        float Default = 0.0f;

        void serialize(Archive& ar) {
            ar << bCooked;
            ar << Default;
        }
    };
} // namespace SatisfactorySave
