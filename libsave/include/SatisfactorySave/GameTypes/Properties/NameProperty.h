#pragma once

#include "../UE/UObject/Name.h"
#include "Property.h"

namespace SatisfactorySave {

    class NameProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& stream) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FName& value() const {
            return value_;
        }

    protected:
        FName value_;
    };
} // namespace SatisfactorySave
