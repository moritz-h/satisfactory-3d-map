#ifndef SATISFACTORY3DMAP_BOOLPROPERTY_H
#define SATISFACTORY3DMAP_BOOLPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class BoolProperty : public Property {
    public:
        using Property::Property;

        void serialize([[maybe_unused]] Archive& ar) override{};

        void accept(PropertyVisitor& v) override;

        [[nodiscard]] uint8_t value() const {
            return tag_.BoolVal;
        }
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BOOLPROPERTY_H
