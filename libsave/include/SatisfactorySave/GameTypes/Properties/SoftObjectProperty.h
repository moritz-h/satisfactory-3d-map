#pragma once

#include "../UE/UObject/SoftObjectPath.h"
#include "Base/Property.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API SoftObjectProperty : public Property {
    public:
        static constexpr std::string_view TypeName = "SoftObjectProperty";

        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FSoftObjectPath& value() const {
            return value_;
        }

    protected:
        FSoftObjectPath value_;
    };
} // namespace SatisfactorySave
