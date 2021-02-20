#ifndef SATISFACTORYSAVEGAME_OBJECTREFERENCE_H
#define SATISFACTORYSAVEGAME_OBJECTREFERENCE_H

#include <string>

namespace SatisfactorySaveGame {

    // FObjectReferenceDisc
    struct ObjectReference {
        std::string level_name;
        std::string path_name;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_OBJECTREFERENCE_H
