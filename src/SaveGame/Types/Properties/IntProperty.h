#ifndef SATISFACTORY3DMAP_INTPROPERTY_H
#define SATISFACTORY3DMAP_INTPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class IntProperty : public Property {
    public:
        IntProperty(std::string property_name, std::string property_type, std::istream& stream);

        int32_t value() const {
            return value_;
        }

    protected:
        int32_t value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_INTPROPERTY_H
