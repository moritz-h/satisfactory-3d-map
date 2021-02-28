#ifndef SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H
#define SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H

#include <cstdint>
#include <istream>
#include <memory>
#include <string>

#include "../Types/ObjectReference.h"
#include "../Types/PropertyCollection.h"

namespace SatisfactorySaveGame {

    // FObjectBaseSaveHeader
    class SaveObjectBase {
    public:
        SaveObjectBase(int32_t type, std::istream& stream);

        virtual void parseData(int32_t length, std::istream& stream);

        [[nodiscard]] int32_t type() const {
            return type_;
        }

        [[nodiscard]] const std::string& className() const {
            return class_name_;
        }

        [[nodiscard]] const ObjectReference& reference() const {
            return reference_;
        }

    protected:
        int32_t type_;
        std::string class_name_;
        ObjectReference reference_;

        std::unique_ptr<PropertyCollection> properties_;
    };
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_SAVEOBJECTBASE_H
