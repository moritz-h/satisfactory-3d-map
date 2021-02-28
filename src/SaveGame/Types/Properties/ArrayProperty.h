#ifndef SATISFACTORYSAVEGAME_ARRAYPROPERTY_H
#define SATISFACTORYSAVEGAME_ARRAYPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class ArrayProperty : public Property {
    public:
        ArrayProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_ARRAYPROPERTY_H
