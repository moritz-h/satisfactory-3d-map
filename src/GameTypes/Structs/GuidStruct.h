#pragma once

#include "../Guid.h"
#include "Struct.h"

namespace Satisfactory3DMap {

    class GuidStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const FGuid& guid() const {
            return guid_;
        }

    protected:
        FGuid guid_;
    };
} // namespace Satisfactory3DMap
