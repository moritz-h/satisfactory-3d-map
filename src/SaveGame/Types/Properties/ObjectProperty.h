#ifndef SATISFACTORYSAVEGAME_OBJECTPROPERTY_H
#define SATISFACTORYSAVEGAME_OBJECTPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class ObjectProperty : public Property {
    public:
        ObjectProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_OBJECTPROPERTY_H
