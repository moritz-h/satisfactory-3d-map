#ifndef SATISFACTORY3DMAP_BYTEARRAY_H
#define SATISFACTORY3DMAP_BYTEARRAY_H

#include <vector>

#include "Array.h"

namespace Satisfactory3DMap {

    class ByteArray : public Array {
    public:
        ByteArray(std::string array_type, int32_t count, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(ArrayVisitor& v) override;

        const std::vector<int8_t>& array() const {
            return array_;
        }

    protected:
        std::vector<int8_t> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BYTEARRAY_H
