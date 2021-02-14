#ifndef SATISFACTORYSAVEGAME_SAVECOMPONENT_H
#define SATISFACTORYSAVEGAME_SAVECOMPONENT_H

#include "SaveObject.h"

namespace SatisfactorySaveGame {

    class SaveComponent : public SaveObject {
    public:
        SaveComponent(int32_t type, std::istream& stream);

    private:
        std::string parent_entity_name_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVECOMPONENT_H
