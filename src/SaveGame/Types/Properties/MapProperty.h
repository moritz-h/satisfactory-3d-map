#ifndef SATISFACTORYSAVEGAME_MAPPROPERTY_H
#define SATISFACTORYSAVEGAME_MAPPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class MapProperty : public Property {
    public:
        MapProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_MAPPROPERTY_H
