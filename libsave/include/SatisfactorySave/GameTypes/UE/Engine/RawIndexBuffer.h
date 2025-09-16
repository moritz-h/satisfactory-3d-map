#pragma once

#include "../../../IO/Archive/Archive.h"
#include "../Core/Containers/DynamicRHIResourceArray.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FRawStaticIndexBuffer {
        TResourceArray IndexStorage;
        bool b32Bit = false;
        bool bShouldExpandTo32Bit = false;

        void serialize(Archive& ar) {
            ar << b32Bit;
            ar << IndexStorage;
            ar << bShouldExpandTo32Bit;
        }
    };
} // namespace SatisfactorySave
