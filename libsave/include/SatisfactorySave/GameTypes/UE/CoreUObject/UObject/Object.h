#pragma once

#include <optional>

#include "../../../../IO/Archive/Archive.h"
#include "../../../Properties/Base/PropertyList.h"
#include "../../Core/Misc/Guid.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API UObject {
    public:
        UObject() = default;
        virtual ~UObject() = default;

        PropertyList Properties;
        std::optional<FGuid> Guid;

        virtual void serialize(Archive& ar) {
            ar << Properties;
            ar << Guid;
        }
    };

} // namespace SatisfactorySave
