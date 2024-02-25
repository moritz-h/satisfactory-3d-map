#pragma once

#include "Property.h"
#include "PropertyVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class PropertyImplBase : public Property {
    public:
        PropertyImplBase() : Property(FName(std::string(Impl::TypeName))) {}
        explicit PropertyImplBase(PropertyTag tag) : Property(std::move(tag)) {}

        void accept(PropertyVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        T Value{};
    };

    // Split into two classes to force subclasses with not serializable type to implement serialize(). Maybe in future
    // override methods support concepts, e.g., `void serialize(Archive& ar) requires IsSerializable<T> override`.
    template<typename Impl, typename T>
    class PropertyImpl : public PropertyImplBase<Impl, T> {
    public:
        using PropertyImplBase<Impl, T>::PropertyImplBase;

        void serialize(Archive& ar) override {
            ar << this->Value;
        }
    };
} // namespace SatisfactorySave
