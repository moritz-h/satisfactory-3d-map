#ifndef SATISFACTORY3DMAP_ENUMPROPERTY_H
#define SATISFACTORY3DMAP_ENUMPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class EnumProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] const FName& enumType() const {
            return tag_.EnumName;
        }

        [[nodiscard]] const FName& value() const {
            return value_;
        }

    protected:
        FName value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ENUMPROPERTY_H
