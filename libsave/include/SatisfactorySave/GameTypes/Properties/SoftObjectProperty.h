#pragma once

#include "../UE/UObject/SoftObjectPath.h"
#include "Property.h"

namespace SatisfactorySave {

    class SoftObjectProperty : public Property {
    public:
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
