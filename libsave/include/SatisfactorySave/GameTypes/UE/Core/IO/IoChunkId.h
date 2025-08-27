#pragma once

#include <array>
#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API FIoChunkId {
    public:
        std::array<uint8_t, 12> Id{};

        void serialize(Archive& ar) {
            ar << Id;
        }
    };
} // namespace SatisfactorySave
