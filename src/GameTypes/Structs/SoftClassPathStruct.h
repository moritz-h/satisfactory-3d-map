#pragma once

#include <string>

#include "../Name.h"
#include "Struct.h"

namespace Satisfactory3DMap {

    class SoftClassPathStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const FName& AssetPathName() const {
            return AssetPathName_;
        }

        [[nodiscard]] const std::string& SubPathString() const {
            return SubPathString_;
        }

    protected:
        FName AssetPathName_;
        std::string SubPathString_;
    };
} // namespace Satisfactory3DMap
