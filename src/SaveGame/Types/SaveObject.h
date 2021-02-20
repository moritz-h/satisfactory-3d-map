#ifndef SATISFACTORYSAVEGAME_SAVEOBJECT_H
#define SATISFACTORYSAVEGAME_SAVEOBJECT_H

#include "SaveObjectBase.h"

namespace SatisfactorySaveGame {

    // FObjectSaveHeader
    class SaveObject : public SaveObjectBase {
    public:
        SaveObject(int32_t type, std::istream& stream);

    private:
        std::string parent_entity_name_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEOBJECT_H
