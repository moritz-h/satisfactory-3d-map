#pragma once

#include "../UE/Satisfactory/ObjectReference.h"
#include "Property.h"

namespace SatisfactorySave {

    class ObjectProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const ObjectReference& value() const {
            return value_;
        }

    protected:
        ObjectReference value_;
    };
} // namespace SatisfactorySave
