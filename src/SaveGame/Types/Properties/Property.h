#ifndef SATISFACTORYSAVEGAME_PROPERTY_H
#define SATISFACTORYSAVEGAME_PROPERTY_H

#include <cstdint>
#include <string>

namespace SatisfactorySaveGame {

    class Property {
    public:
        Property(std::string property_name, std::string property_type, std::istream& stream);

    protected:
        std::string property_name_;
        std::string property_type_;
        int32_t size_;
        int32_t index_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_PROPERTY_H
