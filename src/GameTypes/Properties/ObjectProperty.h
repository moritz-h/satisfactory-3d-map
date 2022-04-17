#pragma once

#include "../ObjectReference.h"
#include "Property.h"

namespace Satisfactory3DMap {

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
} // namespace Satisfactory3DMap
