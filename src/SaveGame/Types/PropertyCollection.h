#ifndef SATISFACTORY3DMAP_PROPERTYCOLLECTION_H
#define SATISFACTORY3DMAP_PROPERTYCOLLECTION_H

#include <cstdint>
#include <istream>
#include <memory>
#include <vector>

#include "Properties/Property.h"

namespace Satisfactory3DMap {

    class PropertyCollection {
    public:
        PropertyCollection(int32_t length, std::istream& stream);

        const std::vector<std::unique_ptr<Property>>& properties() const {
            return properties_;
        }

        const std::vector<char>& extraProperties() const {
            return extraProperties_;
        }

    protected:
        std::unique_ptr<Property> parseProperty(std::istream& stream);

        std::vector<std::unique_ptr<Property>> properties_;
        std::vector<char> extraProperties_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTYCOLLECTION_H
