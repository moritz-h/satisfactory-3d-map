#ifndef SATISFACTORYSAVEGAME_INT64PROPERTY_H
#define SATISFACTORYSAVEGAME_INT64PROPERTY_H

#include "Property.h"

namespace SatisfactorySaveGame {

    class Int64Property : public Property {
    public:
        Int64Property(std::string property_name, std::string property_type, std::istream& stream);

    protected:
        int64_t value_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_INT64PROPERTY_H
