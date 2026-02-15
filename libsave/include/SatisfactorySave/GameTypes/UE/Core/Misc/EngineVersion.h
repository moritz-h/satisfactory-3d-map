#pragma once

#include <cstdint>
#include <string>

#include "../../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API FEngineVersion {
    public:
        uint16_t Major = 0;
        uint16_t Minor = 0;
        uint16_t Patch = 0;
        uint32_t Changelist = 0;
        std::string Branch;

        void serialize(Archive& ar) {
            ar << Major;
            ar << Minor;
            ar << Patch;
            ar << Changelist;
            ar << Branch;
        }
    };
} // namespace SatisfactorySave
