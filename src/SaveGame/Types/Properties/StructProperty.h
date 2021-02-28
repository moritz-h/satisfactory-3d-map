#ifndef SATISFACTORYSAVEGAME_STRUCTPROPERTY_H
#define SATISFACTORYSAVEGAME_STRUCTPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class StructProperty : public Property {
    public:
        StructProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_STRUCTPROPERTY_H
