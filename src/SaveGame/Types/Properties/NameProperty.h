#ifndef SATISFACTORY3DMAP_NAMEPROPERTY_H
#define SATISFACTORY3DMAP_NAMEPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class NameProperty : public Property {
    public:
        NameProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
        std::string value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_NAMEPROPERTY_H
