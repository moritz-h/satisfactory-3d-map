#ifndef SATISFACTORYSAVEGAME_PROPERTYCOLLECTION_H
#define SATISFACTORYSAVEGAME_PROPERTYCOLLECTION_H

#include <cstdint>
#include <istream>
#include <memory>

#include "Properties/Property.h"

namespace SatisfactorySaveGame {

    class PropertyCollection {
    public:
        PropertyCollection(int32_t length, std::istream& stream);

    protected:
        std::unique_ptr<Property> parseProperty(std::istream& stream);
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_PROPERTYCOLLECTION_H
