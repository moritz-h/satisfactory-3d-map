#ifndef SATISFACTORY3DMAP_COLORSTRUCT_H
#define SATISFACTORY3DMAP_COLORSTRUCT_H

#include "Struct.h"
#include "Utils/StreamUtils.h"

namespace Satisfactory3DMap {

    class ColorStruct : public Struct {
    public:
        ColorStruct(std::string struct_name, std::istream& stream) : Struct(std::move(struct_name)) {
            b_ = read<int8_t>(stream);
            g_ = read<int8_t>(stream);
            r_ = read<int8_t>(stream);
            a_ = read<int8_t>(stream);
        }

    protected:
        int8_t b_;
        int8_t g_;
        int8_t r_;
        int8_t a_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_COLORSTRUCT_H
