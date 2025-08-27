#pragma once

#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API FIoContainerId {
    public:
        static constexpr uint64_t InvalidId = static_cast<uint64_t>(-1);

        uint64_t Id = InvalidId;

        void serialize(Archive& ar) {
            ar << Id;
        }
    };
} // namespace SatisfactorySave
