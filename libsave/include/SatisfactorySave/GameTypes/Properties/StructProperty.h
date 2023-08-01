#pragma once

#include <vector>

#include "../Structs/Base/Struct.h"
#include "../UE/Misc/Guid.h"
#include "Base/Property.h"

namespace SatisfactorySave {

    class StructProperty : public Property {
    public:
        static constexpr std::string_view TypeName = "StructProperty";

        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const std::string& structName() const {
            return Tag.StructName.Name;
        }

        [[nodiscard]] const FGuid& guid() const {
            return Tag.StructGuid;
        }

        [[nodiscard]] const std::unique_ptr<Struct>& value() const {
            return struct_;
        }

    protected:
        std::unique_ptr<Struct> struct_;
    };
} // namespace SatisfactorySave
