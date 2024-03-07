#pragma once

#include "../UE/Satisfactory/ObjectReference.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ObjectProperty final : public PropertyImpl<ObjectProperty, FObjectReferenceDisc> {
    public:
        static constexpr std::string_view TypeName = "ObjectProperty";

        using PropertyImpl<ObjectProperty, FObjectReferenceDisc>::PropertyImpl;
    };
} // namespace SatisfactorySave
