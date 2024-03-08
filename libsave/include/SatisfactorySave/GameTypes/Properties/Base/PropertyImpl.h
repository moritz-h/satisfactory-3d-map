#pragma once

#include "Property.h"
#include "PropertyVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class PropertyImplBase : public Property {
    public:
        PropertyImplBase() : Property(FName(std::string(Impl::TypeName))) {}
        explicit PropertyImplBase(PropertyTag tag) : Property(std::move(tag)) {}

        [[nodiscard]] std::unique_ptr<Property> clone() const override {
            return std::make_unique<Impl>(*dynamic_cast<const Impl*>(this));
        };

        void accept(PropertyVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        T Value{};
    };

    template<typename Impl, typename T>
    class PropertyImplBase<Impl, std::unique_ptr<T>> : public Property {
    public:
        PropertyImplBase() : Property(FName(std::string(Impl::TypeName))) {}
        explicit PropertyImplBase(PropertyTag tag) : Property(std::move(tag)) {}

        PropertyImplBase(const PropertyImplBase& other) : Property(other) {
            if (other.Value) {
                Value = other.Value->clone();
            }
        }

        PropertyImplBase& operator=(const PropertyImplBase& other) {
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

        PropertyImplBase(PropertyImplBase&&) = default;
        PropertyImplBase& operator=(PropertyImplBase&&) = default;

        [[nodiscard]] std::unique_ptr<Property> clone() const override {
            return std::make_unique<Impl>(*dynamic_cast<const Impl*>(this));
        };

        void accept(PropertyVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        std::unique_ptr<T> Value{};
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
