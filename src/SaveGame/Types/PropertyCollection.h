#ifndef SATISFACTORYSAVEGAME_PROPERTYCOLLECTION_H
#define SATISFACTORYSAVEGAME_PROPERTYCOLLECTION_H

#include <istream>

namespace SatisfactorySaveGame {

    class PropertyCollection {
    public:
        PropertyCollection(int32_t length, std::istream& stream);

    protected:
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_PROPERTYCOLLECTION_H
