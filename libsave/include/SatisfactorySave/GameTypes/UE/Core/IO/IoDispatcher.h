#pragma once

#include <cstdint>

namespace SatisfactorySave {
    enum class EIoContainerFlags : uint8_t {
        None,
        Compressed = 1 << 0,
        Encrypted = 1 << 1,
        Signed = 1 << 2,
        Indexed = 1 << 3,
        OnDemand = 1 << 4,
    };
} // namespace SatisfactorySave
