#ifndef SATISFACTORY3DMAP_OBJECTARRAY_H
#define SATISFACTORY3DMAP_OBJECTARRAY_H

#include <vector>

#include "../ObjectReference.h"
#include "Array.h"

namespace Satisfactory3DMap {

    class ObjectArray : public Array {
    public:
        using Array::Array;

        void serialize(Archive& ar) override;

        void accept(ArrayVisitor& v) override;

        [[nodiscard]] const std::vector<ObjectReference>& array() const {
            return array_;
        }

    protected:
        std::vector<ObjectReference> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_OBJECTARRAY_H
