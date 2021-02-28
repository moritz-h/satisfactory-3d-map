#ifndef SATISFACTORYSAVEGAME_BYTEPROPERTY_H
#define SATISFACTORYSAVEGAME_BYTEPROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class ByteProperty : public Property {
    public:
        ByteProperty(std::string property_name, std::string property_type, std::istream& stream);

    protected:
        std::string byte_type_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_BYTEPROPERTY_H
