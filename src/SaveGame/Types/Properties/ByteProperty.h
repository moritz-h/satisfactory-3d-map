#ifndef SATISFACTORY3DMAP_BYTEPROPERTY_H
#define SATISFACTORY3DMAP_BYTEPROPERTY_H

#include <vector>

#include "Property.h"

namespace Satisfactory3DMap {

    class ByteProperty : public Property {
    public:
        ByteProperty(std::string property_name, std::string property_type, std::istream& stream);

        const std::string& byteType() const {
            return byte_type_;
        }

        const std::string& value() const {
            return value_;
        }

    protected:
        std::string byte_type_;

        std::string value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BYTEPROPERTY_H
