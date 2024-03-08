#pragma once

#include <vector>

#include "Set.h"
#include "SetVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class SetImplBase : public Set {
    public:
        std::unique_ptr<Set> clone() override {
            return std::make_unique<Impl>(*dynamic_cast<Impl*>(this));
        };

        void accept(SetVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        std::vector<T> Values;
    };

    template<typename Impl, typename T>
    class SetImplBase<Impl, std::unique_ptr<T>> : public Set {
    public:
        SetImplBase() = default;

        SetImplBase(const SetImplBase& other) : Set(other) {
            Values.reserve(other.Values.size());
            for (const auto& s : other.Values) {
                Values.push_back(std::move(s->clone()));
            }
        }

        SetImplBase& operator=(const SetImplBase& other) {
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

        SetImplBase(SetImplBase&&) = default;
        SetImplBase& operator=(SetImplBase&&) = default;

        std::unique_ptr<Set> clone() override {
            return std::make_unique<Impl>(*dynamic_cast<Impl*>(this));
        };

        void accept(SetVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        std::vector<std::unique_ptr<T>> Values;
    };

    template<typename Impl, typename T>
    class SetImpl : public SetImplBase<Impl, T> {
    public:
        void serialize(Archive& ar) override {
            ar << this->Values;
        }
    };
} // namespace SatisfactorySave
