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

        void accept(ArrayVisitor& v) override;

    protected:
        std::string name_;
        std::string struct_name_;
        Guid guid_;
        std::vector<std::unique_ptr<Struct>> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCTARRAY_H
