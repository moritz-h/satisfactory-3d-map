#ifndef SATISFACTORYSAVEGAME_ENUMPROPERTY_H
#define SATISFACTORYSAVEGAME_ENUMPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class EnumProperty : public Property {
    public:
        EnumProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
        std::string enum_type_;
        std::string value_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_ENUMPROPERTY_H
