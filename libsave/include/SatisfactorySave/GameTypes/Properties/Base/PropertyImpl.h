#pragma once

#include "../../../Utils/Concepts.h"
#include "Property.h"
#include "PropertyVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class PropertyImplBase : public Property {
    public:
        using value_type = T;

        PropertyImplBase() : Property(FName(std::string(Impl::TypeName))) {}
        explicit PropertyImplBase(FName name) : Property(FName(std::string(Impl::TypeName)), std::move(name)) {}
        PropertyImplBase(FName name, T value)
            : Property(FName(std::string(Impl::TypeName)), std::move(name)),
              Value(std::move(value)) {}
        explicit PropertyImplBase(PropertyTag tag) : Property(std::move(tag)) {}

        PropertyImplBase(const PropertyImplBase&) = default;
        PropertyImplBase& operator=(const PropertyImplBase&) = default;
        PropertyImplBase(PropertyImplBase&&) = default;
        PropertyImplBase& operator=(PropertyImplBase&&) = default;

        // Deep copy shared_ptr
        PropertyImplBase(const PropertyImplBase& other) requires IsSharedPtr<T> : Property(other) {
            if (other.Value) {
                Value = other.Value->clone();
            }
        }

        // Deep copy shared_ptr
        PropertyImplBase& operator=(const PropertyImplBase& other) requires IsSharedPtr<T> {
            if (this != &other) {
                Property::operator=(other);
                if (other.Value) {
                    Value = other.Value->clone();
                } else {
                    Value.reset();
                }
            }
            return *this;
        }

        [[nodiscard]] std::shared_ptr<Property> clone() const override {
            return std::make_shared<Impl>(*dynamic_cast<const Impl*>(this));
        }

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
