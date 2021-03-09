#ifndef SATISFACTORY3DMAP_PROPERTYCOLLECTION_H
#define SATISFACTORY3DMAP_PROPERTYCOLLECTION_H

#include <cstdint>
#include <istream>
#include <memory>

#include "Properties/Property.h"

namespace Satisfactory3DMap {

    class PropertyCollection {
    public:
        PropertyCollection(int32_t length, std::istream& stream);

    protected:
        std::unique_ptr<Property> parseProperty(std::istream& stream);
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTYCOLLECTION_H
