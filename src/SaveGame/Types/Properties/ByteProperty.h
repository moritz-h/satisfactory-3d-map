#ifndef SATISFACTORY3DMAP_BYTEPROPERTY_H
#define SATISFACTORY3DMAP_BYTEPROPERTY_H

#include "Property.h"

namespace Satisfactory3DMap {

    class ByteProperty : public Property {
    public:
        ByteProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
        std::string byte_type_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BYTEPROPERTY_H
