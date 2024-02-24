#pragma once

#include <vector>

#include "../Structs/Base/Struct.h"
#include "../UE/Misc/Guid.h"
#include "Base/Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API StructProperty : public Property {
    public:
        static constexpr std::string_view TypeName = "StructProperty";

        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] inline FName& structName() {
            return tag_.StructName;
        }

        [[nodiscard]] inline FGuid& structGuid() {
            return tag_.StructGuid;
        }

        [[nodiscard]] const std::unique_ptr<Struct>& value() const {
            return struct_;
        }

    protected:
        std::unique_ptr<Struct> struct_;
    };
} // namespace SatisfactorySave
