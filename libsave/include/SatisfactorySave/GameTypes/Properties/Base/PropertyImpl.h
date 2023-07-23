#pragma once

#include "Property.h"
#include "PropertyVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class PropertyImpl : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override {
            ar << Value;
        }

        void accept(PropertyVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        T Value{};
    };
} // namespace SatisfactorySave
