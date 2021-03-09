#ifndef SATISFACTORY3DMAP_PROPERTY_H
#define SATISFACTORY3DMAP_PROPERTY_H

#include <cstdint>
#include <string>

namespace Satisfactory3DMap {

    class Property {
    public:
        Property(std::string property_name, std::string property_type, std::istream& stream);

    protected:
        std::string property_name_;
        std::string property_type_;
        int32_t size_;
        int32_t index_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTY_H
