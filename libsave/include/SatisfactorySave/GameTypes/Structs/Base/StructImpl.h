#pragma once

#include "../../../IO/Archive/IStreamArchive.h"
#include "Struct.h"
#include "StructVisitor.h"

namespace SatisfactorySave {

    template<typename Impl, typename T>
    class StructImplBase : public Struct {
    public:
        using Struct::Struct;

        [[nodiscard]] std::shared_ptr<Struct> clone() const override {
            return std::make_shared<Impl>(*dynamic_cast<const Impl*>(this));
        }

        void serialize(Archive& ar) override {
            if (ar.isIArchive()) {
                auto& inAr = dynamic_cast<IStreamArchive&>(ar);
                auto parent_info_stack_pusher = inAr.pushParentClassInfo(name_.toString());
                inAr << Data;
            } else {
                ar << Data;
            }
        }

        void accept(StructVisitor& v) override {
            v.visit(static_cast<Impl&>(*this));
        }

        T Data;
    };

    template<typename Impl, typename T>
    class StructImpl : public StructImplBase<Impl, T> {
    public:
        using StructImplBase<Impl, T>::StructImplBase;
        StructImpl() : StructImplBase<Impl, T>(FName(std::string(Impl::TypeName))) {}
    };
} // namespace SatisfactorySave
