#pragma once

#include "../UE/Satisfactory/ObjectReference.h"
#include "Base/Struct.h"

namespace SatisfactorySave {

    class InventoryItemStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] int32_t unk1() const {
            return unk1_;
        }

        [[nodiscard]] const std::string& className() const {
            return class_name_;
        }

        [[nodiscard]] const ObjectReference& ref() const {
            return ref_;
        }

    protected:
        int32_t unk1_ = 0;
        std::string class_name_;
        ObjectReference ref_;
    };
} // namespace SatisfactorySave
