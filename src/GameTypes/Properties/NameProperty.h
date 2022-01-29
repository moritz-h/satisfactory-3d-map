#ifndef SATISFACTORY3DMAP_NAMEPROPERTY_H
#define SATISFACTORY3DMAP_NAMEPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

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
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_NAMEPROPERTY_H
