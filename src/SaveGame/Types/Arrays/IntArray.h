#ifndef SATISFACTORY3DMAP_INTARRAY_H
#define SATISFACTORY3DMAP_INTARRAY_H

#include <vector>

#include "Array.h"

namespace Satisfactory3DMap {

    class IntArray : public Array {
    public:
        IntArray(std::string array_type, int32_t count, std::istream& stream);

        void accept(ArrayVisitor& v) override;

        const std::vector<int32_t>& array() const {
            return array_;
        }

    protected:
        std::vector<int32_t> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_INTARRAY_H
