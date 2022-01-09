#ifndef SATISFACTORY3DMAP_INT8PROPERTY_H
#define SATISFACTORY3DMAP_INT8PROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class Int8Property : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] int8_t value() const {
            return value_;
        }

    protected:
        int8_t value_ = 0;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_INT8PROPERTY_H
