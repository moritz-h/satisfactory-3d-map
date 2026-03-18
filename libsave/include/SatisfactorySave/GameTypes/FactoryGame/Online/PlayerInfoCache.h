#pragma once

#include <cstdint>

#include "../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FPlayerInfoHandle {
    public:
        uint8_t ServiceProvider = 0;
        int32_t PlayerInfoTableIndex = -1;

        void serialize(Archive& ar) {
            if (ar.getSaveVersion() >= 57 && ar.getSaveVersion() < 58) {
                ar << ServiceProvider;
                ar << PlayerInfoTableIndex;
            }

            if (ar.getSaveVersion() >= 57) {
                ar << ServiceProvider;
                ar << PlayerInfoTableIndex;
            } else {
                ar << ServiceProvider;
                uint8_t LegacyPlayerInfoTableIndex =
                    (PlayerInfoTableIndex == -1) ? 0 : static_cast<uint8_t>(PlayerInfoTableIndex);
                ar << LegacyPlayerInfoTableIndex;
                PlayerInfoTableIndex = LegacyPlayerInfoTableIndex;
                if (ServiceProvider == 0) {
                    PlayerInfoTableIndex = -1;
                }
            }
        }
    };
} // namespace SatisfactorySave
