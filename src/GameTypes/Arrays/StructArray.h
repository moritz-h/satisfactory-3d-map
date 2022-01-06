#ifndef SATISFACTORY3DMAP_STRUCTARRAY_H
#define SATISFACTORY3DMAP_STRUCTARRAY_H

#include <vector>

#include "../Guid.h"
#include "../Structs/Struct.h"
#include "Array.h"

namespace Satisfactory3DMap {

    class StructArray : public Array {
    public:
        StructArray(std::string array_type, int32_t count, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(ArrayVisitor& v) override;

        const std::string& name() const {
            return name_;
        }

        const std::string& structName() const {
            return struct_name_;
        }

        std::string guid() const {
            return guid_.toString();
        }

        const std::vector<std::unique_ptr<Struct>>& array() const {
            return array_;
        }

    protected:
        std::string name_;
        std::string struct_name_;
        Guid guid_;
        std::vector<std::unique_ptr<Struct>> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCTARRAY_H
