#ifndef SATISFACTORY3DMAP_INTARRAY_H
#define SATISFACTORY3DMAP_INTARRAY_H

#include <vector>

#include "Array.h"

namespace Satisfactory3DMap {

    class IntArray : public Array {
    public:
        using Array::Array;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] const std::vector<int32_t>& array() const {
            return array_;
        }

    protected:
        std::vector<int32_t> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_INTARRAY_H
