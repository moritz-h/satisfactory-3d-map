#pragma once

#include <vector>

#include "Set.h"
#include "SetVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class SetImplBase : public Set {
    public:
        void accept(SetVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        std::vector<T> Set;
    };

    template<typename Impl, typename T>
    class SetImpl : public SetImplBase<Impl, T> {
    public:
        void serialize(Archive& ar) override {
            ar << this->Set;
        }
    };
} // namespace SatisfactorySave
