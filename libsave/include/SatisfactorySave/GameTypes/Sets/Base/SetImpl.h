#pragma once

#include <vector>

#include "../../../Utils/Concepts.h"
#include "Set.h"
#include "SetVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class SetImplBase : public Set {
    public:
        SetImplBase() = default;
        SetImplBase(const SetImplBase&) = default;
        SetImplBase& operator=(const SetImplBase&) = default;
        SetImplBase(SetImplBase&&) = default;
        SetImplBase& operator=(SetImplBase&&) = default;

        SetImplBase(const SetImplBase& other) requires IsUniquePtr<T> : Set(other) {
            Values.reserve(other.Values.size());
            for (const auto& s : other.Values) {
                Values.push_back(std::move(s->clone()));
            }
        }

        SetImplBase& operator=(const SetImplBase& other) requires IsUniquePtr<T> {
            if (this != &other) {
                Set::operator=(other);
                Values.clear();
                Values.reserve(other.Values.size());
                for (const auto& s : other.Values) {
                    Values.push_back(std::move(s->clone()));
                }
            }
            return *this;
        }

        [[nodiscard]] std::unique_ptr<Set> clone() const override {
            return std::make_unique<Impl>(*dynamic_cast<const Impl*>(this));
        };

        void accept(SetVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        std::vector<T> Values;
    };

    template<typename Impl, typename T>
    class SetImpl : public SetImplBase<Impl, T> {
    public:
        void serialize(Archive& ar) override {
            ar << this->Values;
        }
    };
} // namespace SatisfactorySave
