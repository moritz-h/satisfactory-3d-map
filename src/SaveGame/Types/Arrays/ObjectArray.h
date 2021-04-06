#ifndef SATISFACTORY3DMAP_OBJECTARRAY_H
#define SATISFACTORY3DMAP_OBJECTARRAY_H

#include <vector>

#include "../ObjectReference.h"
#include "Array.h"

namespace Satisfactory3DMap {

    class ObjectArray : public Array {
    public:
        ObjectArray(std::string array_type, int32_t count, std::istream& stream);

        void accept(ArrayVisitor& v) override;

    protected:
        std::vector<ObjectReference> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_OBJECTARRAY_H
