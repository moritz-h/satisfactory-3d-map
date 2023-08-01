#pragma once

#include <vector>

#include "Set.h"
#include "SetVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class SetImpl : public Set {
    public:
        void serialize(Archive& ar) override {
            ar << Set;
        }

        void accept(SetVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        std::vector<T> Set;
    };
} // namespace SatisfactorySave
