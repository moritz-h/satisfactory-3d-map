#ifndef SATISFACTORYSAVEGAME_STRPROPERTY_H
#define SATISFACTORYSAVEGAME_STRPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class StrProperty : public Property {
    public:
        StrProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_STRPROPERTY_H
