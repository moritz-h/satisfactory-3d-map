#pragma once

#include "../Arrays/Base/Array.h"
#include "Base/Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ArrayProperty : public Property {
    public:
        static constexpr std::string_view TypeName = "ArrayProperty";

        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] inline FName& arrayType() {
            return tag_.InnerType;
        }

        [[nodiscard]] const std::unique_ptr<Array>& array() const {
            return array_;
        }

    protected:
        std::unique_ptr<Array> array_;
    };
} // namespace SatisfactorySave
