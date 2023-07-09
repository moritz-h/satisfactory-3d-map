#pragma once

#include <vector>

#include "../Properties/PropertyTag.h"
#include "../Structs/Base/Struct.h"
#include "../UE/Misc/Guid.h"
#include "Base/Array.h"

namespace SatisfactorySave {

    class StructArray : public Array {
    public:
        static constexpr std::string_view TypeName = "StructProperty";

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

        std::vector<std::unique_ptr<Struct>> Values;

    protected:
        PropertyTag inner_tag_;
    };
} // namespace SatisfactorySave
