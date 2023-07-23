#pragma once

#include "../UE/Satisfactory/ObjectReference.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class ObjectProperty : public PropertyImpl<ObjectProperty, ObjectReference> {
    public:
        static constexpr std::string_view TypeName = "ObjectProperty";

        using PropertyImpl<ObjectProperty, ObjectReference>::PropertyImpl;
    };
} // namespace SatisfactorySave
