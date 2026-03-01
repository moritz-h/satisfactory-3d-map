#pragma once

#include <array>
#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API FIoHash {
    public:
        std::array<uint8_t, 20> Hash{};

        void serialize(Archive& ar) {
            ar << Hash;
        }
    };
} // namespace SatisfactorySave
