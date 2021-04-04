#ifndef SATISFACTORY3DMAP_SAVEOBJECTBASE_H
#define SATISFACTORY3DMAP_SAVEOBJECTBASE_H

#include <cstdint>
#include <istream>
#include <memory>
#include <string>

#include "../Types/ObjectReference.h"
#include "../Types/Properties/Property.h"

namespace Satisfactory3DMap {

    // FObjectBaseSaveHeader
    class SaveObjectBase {
    public:
        SaveObjectBase(int32_t id, int32_t type, std::istream& stream);
        virtual ~SaveObjectBase() = default;

        virtual void parseData(int32_t length, std::istream& stream);

        [[nodiscard]] int32_t id() const {
            return id_;
        }

        [[nodiscard]] int32_t type() const {
            return type_;
        }

        [[nodiscard]] const std::string& className() const {
            return class_name_;
        }

        [[nodiscard]] const ObjectReference& reference() const {
            return reference_;
        }

        const std::vector<std::unique_ptr<Property>>& properties() const {
            return properties_;
        }

        const std::vector<char>& extraProperties() const {
            return extraProperties_;
        }

    protected:
        int32_t id_;
        int32_t type_;
        std::string class_name_;
        ObjectReference reference_;

        std::vector<std::unique_ptr<Property>> properties_;
        std::vector<char> extraProperties_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_SAVEOBJECTBASE_H
