#ifndef SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H
#define SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H

#include <istream>
#include <memory>
#include <string>

#include "ObjectReference.h"
#include "PropertyCollection.h"

namespace SatisfactorySaveGame {

    // FObjectBaseSaveHeader
    class SaveObjectBase {
    public:
        SaveObjectBase(int32_t type, std::istream& stream);

        virtual void parseData(int32_t length, std::istream& stream);

    protected:
        int32_t type_;
        std::string class_name_;
        ObjectReference reference_;

        std::unique_ptr<PropertyCollection> properties_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H
