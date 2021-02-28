#ifndef SATISFACTORYSAVEGAME_BOOLPROPERTY_H
#define SATISFACTORYSAVEGAME_BOOLPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class BoolProperty : public Property {
    public:
        BoolProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
        int8_t value_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_BOOLPROPERTY_H
