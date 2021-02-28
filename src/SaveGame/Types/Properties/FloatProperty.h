#ifndef SATISFACTORYSAVEGAME_FLOATPROPERTY_H
#define SATISFACTORYSAVEGAME_FLOATPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class FloatProperty : public Property {
    public:
        FloatProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_FLOATPROPERTY_H
