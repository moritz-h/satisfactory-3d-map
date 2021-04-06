#ifndef SATISFACTORY3DMAP_BOOLPROPERTY_H
#define SATISFACTORY3DMAP_BOOLPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class BoolProperty : public Property {
    public:
        BoolProperty(std::string property_name, std::string property_type, std::istream& stream);

        void accept(PropertyVisitor& v) override;

        int8_t value() const {
            return value_;
        }

    protected:
        int8_t value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BOOLPROPERTY_H
