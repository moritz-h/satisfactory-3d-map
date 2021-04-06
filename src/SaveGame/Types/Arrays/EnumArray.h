#ifndef SATISFACTORY3DMAP_ENUMARRAY_H
#define SATISFACTORY3DMAP_ENUMARRAY_H

#include <string>
#include <vector>

#include "Array.h"

namespace Satisfactory3DMap {

    class EnumArray : public Array {
    public:
        EnumArray(std::string array_type, int32_t count, std::istream& stream);

        void accept(ArrayVisitor& v) override;

        const std::vector<std::string>& array() const {
            return array_;
        }

    protected:
        std::vector<std::string> array_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ENUMARRAY_H
