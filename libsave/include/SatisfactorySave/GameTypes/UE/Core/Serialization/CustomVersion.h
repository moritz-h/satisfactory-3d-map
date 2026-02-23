#pragma once

#include <cstdint>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../Misc/Guid.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FCustomVersion {
        FGuid Key;
        int32_t Version = 0;

        void serialize(Archive& ar) {
            ar << Key;
            ar << Version;
        }
    };

    using FCustomVersionArray = std::vector<FCustomVersion>;

    class SATISFACTORYSAVE_API FCustomVersionContainer {
    public:
        FCustomVersionArray Versions;

        void serialize(Archive& ar) {
            ar << Versions;
        }
    };
} // namespace SatisfactorySave
