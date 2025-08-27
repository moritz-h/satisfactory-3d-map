#pragma once

#include <array>
#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API FIoChunkHash {
    public:
        std::array<uint8_t, 32> Hash{};

        void serialize(Archive& ar) {
            ar << Hash;
        }
    };

    enum class EIoContainerFlags : uint8_t {
        None,
        Compressed = 1 << 0,
        Encrypted = 1 << 1,
        Signed = 1 << 2,
        Indexed = 1 << 3,
        OnDemand = 1 << 4,
    };
} // namespace SatisfactorySave
