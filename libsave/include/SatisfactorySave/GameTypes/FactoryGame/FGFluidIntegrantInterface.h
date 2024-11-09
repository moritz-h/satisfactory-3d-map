#pragma once

#include "../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FFluidBox {
    public:
        float Value;

        void serialize(Archive& ar) {
            ar << Value;
        }
    };
} // namespace SatisfactorySave
