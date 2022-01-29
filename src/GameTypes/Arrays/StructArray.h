#ifndef SATISFACTORY3DMAP_STRUCTARRAY_H
#define SATISFACTORY3DMAP_STRUCTARRAY_H

#include <vector>

#include "../Guid.h"
#include "../Structs/Struct.h"
#include "Array.h"

namespace Satisfactory3DMap {

    class StructArray : public Array {
    public:
        using Array::Array;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] const FName& name() const {
            return name_;
        }

        [[nodiscard]] const FName& structName() const {
            return struct_name_;
        }

        [[nodiscard]] const Guid& guid() const {
            return guid_;
        }

        [[nodiscard]] const std::vector<std::unique_ptr<Struct>>& array() const {
            return array_;
        }

    protected:
        FName name_;
        FName struct_name_;
        Guid guid_;
        std::vector<std::unique_ptr<Struct>> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCTARRAY_H
