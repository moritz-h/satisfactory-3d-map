#ifndef SATISFACTORYSAVEGAME_TEXTPROPERTY_H
#define SATISFACTORYSAVEGAME_TEXTPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class TextProperty : public Property {
    public:
        TextProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_TEXTPROPERTY_H
