#pragma once

#include <vector>

#include "../Misc/Guid.h"
#include "../Properties/PropertyTag.h"
#include "../Structs/Struct.h"
#include "Array.h"

namespace Satisfactory3DMap {

    class StructArray : public Array {
    public:
        using Array::Array;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] const FName& name() const {
            return inner_tag_.Name;
        }

        [[nodiscard]] const FName& structName() const {
            return inner_tag_.StructName;
        }

        [[nodiscard]] const FGuid& structGuid() const {
            return inner_tag_.StructGuid;
        }

        [[nodiscard]] const std::vector<std::unique_ptr<Struct>>& array() const {
            return array_;
        }

    protected:
        PropertyTag inner_tag_;
        std::vector<std::unique_ptr<Struct>> array_;
    };
} // namespace Satisfactory3DMap
