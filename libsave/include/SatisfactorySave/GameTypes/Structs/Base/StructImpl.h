#pragma once

#include "../../../IO/Archive/IStreamArchive.h"
#include "Struct.h"
#include "StructVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class StructImpl : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override {
            if (ar.isIArchive()) {
                auto& inAr = dynamic_cast<IStreamArchive&>(ar);
                inAr.ParentClassInfo.push(name_.toString());
                inAr << Data;
                inAr.ParentClassInfo.pop();
            } else {
                ar << Data;
            }
        }

        void accept(StructVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        T Data;
    };
} // namespace SatisfactorySave
