#ifndef SATISFACTORYSAVEGAME_SAVEOBJECT_H
#define SATISFACTORYSAVEGAME_SAVEOBJECT_H

#include "SaveObjectBase.h"

namespace SatisfactorySaveGame {

    // FObjectSaveHeader
    class SaveObject : public SaveObjectBase {
    public:
        SaveObject(int32_t type, std::istream& stream);

        void parseData(int32_t length, std::istream& stream) override;

    protected:
        std::string outer_path_name_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEOBJECT_H
