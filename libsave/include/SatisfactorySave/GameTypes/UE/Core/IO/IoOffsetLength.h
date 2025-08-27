#pragma once

#include <array>
#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FIoOffsetAndLength {
        std::array<uint8_t, 10> OffsetAndLength{};

        [[nodiscard]] inline uint64_t GetOffset() const {
            return static_cast<uint64_t>(OffsetAndLength[4]) | static_cast<uint64_t>(OffsetAndLength[3]) << 8 |
                   static_cast<uint64_t>(OffsetAndLength[2]) << 16 | static_cast<uint64_t>(OffsetAndLength[1]) << 24 |
                   static_cast<uint64_t>(OffsetAndLength[0]) << 32;
        }

        [[nodiscard]] inline uint64_t GetLength() const {
            return static_cast<uint64_t>(OffsetAndLength[9]) | static_cast<uint64_t>(OffsetAndLength[8]) << 8 |
                   static_cast<uint64_t>(OffsetAndLength[7]) << 16 | static_cast<uint64_t>(OffsetAndLength[6]) << 24 |
                   static_cast<uint64_t>(OffsetAndLength[5]) << 32;
        }

        void serialize(Archive& ar) {
            ar << OffsetAndLength;
        }
    };
} // namespace SatisfactorySave
