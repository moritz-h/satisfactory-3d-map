#pragma once

#include "Struct.h"
#include "StructVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class StructImpl : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override {
            ar << Data;
        }

        void accept(StructVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        T Data;
    };
} // namespace SatisfactorySave
