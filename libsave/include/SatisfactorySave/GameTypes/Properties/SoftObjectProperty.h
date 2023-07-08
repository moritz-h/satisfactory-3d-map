#pragma once

#include "../Misc/SoftObjectPath.h"
#include "Property.h"

namespace SatisfactorySave {

    class SoftObjectProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const SoftObjectPath& value() const {
            return value_;
        }

    protected:
        SoftObjectPath value_;
    };
} // namespace SatisfactorySave
