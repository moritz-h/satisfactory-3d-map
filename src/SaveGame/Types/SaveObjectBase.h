#ifndef SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H
#define SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H

#include <istream>
#include <string>

#include "ObjectReference.h"

namespace SatisfactorySaveGame {

    // FObjectBaseSaveHeader
    class SaveObjectBase {
    public:
        SaveObjectBase(int32_t type, std::istream& stream);

        virtual void parseData(int32_t length, std::istream& stream) = 0;

    protected:
        int32_t type_;
        std::string class_name_;
        ObjectReference reference_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H
