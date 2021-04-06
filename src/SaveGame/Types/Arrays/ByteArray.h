#ifndef SATISFACTORY3DMAP_BYTEARRAY_H
#define SATISFACTORY3DMAP_BYTEARRAY_H

#include <vector>

#include "Array.h"

namespace Satisfactory3DMap {

    class ByteArray : public Array {
    public:
        ByteArray(std::string array_type, int32_t count, std::istream& stream);

        void accept(ArrayVisitor& v) override;

    protected:
        std::vector<int8_t> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BYTEARRAY_H
