#pragma once

#include <string>

#include "../../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FLocalUserNetIdBundle {
    public:
        std::string AssociatedAccountIdString;
        std::string DisplayName;
        std::string BackendName;

        void serialize(Archive& ar) {
            ar << AssociatedAccountIdString;
            ar << DisplayName;
            ar << BackendName;
        }
    };

} // namespace SatisfactorySave
