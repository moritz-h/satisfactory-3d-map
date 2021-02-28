#ifndef SATISFACTORYSAVEGAME_NAMEPROPERTY_H
#define SATISFACTORYSAVEGAME_NAMEPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class NameProperty : public Property {
    public:
        NameProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
        std::string value_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_NAMEPROPERTY_H
