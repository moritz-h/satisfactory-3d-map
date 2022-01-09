#ifndef SATISFACTORY3DMAP_FLOATPROPERTY_H
#define SATISFACTORY3DMAP_FLOATPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class FloatProperty : public Property {
    public:
        using Property::Property;

        void serialize(Archive& ar) override;

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] float value() const {
            return value_;
        }

    protected:
        float value_ = 0.0f;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_FLOATPROPERTY_H
