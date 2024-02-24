#pragma once

#include "Property.h"
#include "PropertyVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class PropertyImpl : public Property {
    public:
        PropertyImpl() : Property(FName(std::string(Impl::TypeName))) {}
        explicit PropertyImpl(PropertyTag tag) : Property(std::move(tag)) {}

        void serialize(Archive& ar) override {
            ar << Value;
        }

        void accept(PropertyVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        T Value{};
    };
} // namespace SatisfactorySave
