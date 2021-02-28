#ifndef SATISFACTORYSAVEGAME_INTPROPERTY_H
#define SATISFACTORYSAVEGAME_INTPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class IntProperty : public Property {
    public:
        IntProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
        int32_t value_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_INTPROPERTY_H
