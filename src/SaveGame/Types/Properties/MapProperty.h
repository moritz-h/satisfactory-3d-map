#ifndef SATISFACTORY3DMAP_MAPPROPERTY_H
#define SATISFACTORY3DMAP_MAPPROPERTY_H

#include <vector>

#include "Property.h"

namespace Satisfactory3DMap {

    class MapProperty : public Property {
    public:
        MapProperty(std::string property_name, std::string property_type, std::istream& stream);

        const std::vector<char>& buf() const {
            return buf_;
        }

    protected:
        std::string key_type_;
        std::string value_type_;

        std::vector<char> buf_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPPROPERTY_H
