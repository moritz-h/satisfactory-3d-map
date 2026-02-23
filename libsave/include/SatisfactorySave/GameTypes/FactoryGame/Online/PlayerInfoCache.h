#pragma once

#include <cstdint>

#include "../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FPlayerInfoHandle {
    public:
        uint8_t ServiceProvider = 0;
        uint8_t PlayerInfoTableIndex = 0;

        void serialize(Archive& ar) {
            ar << ServiceProvider;
            ar << PlayerInfoTableIndex;
        }
    };
} // namespace SatisfactorySave
