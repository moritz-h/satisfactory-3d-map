#ifndef SATISFACTORY3DMAP_LINEARCOLORSTRUCT_H
#define SATISFACTORY3DMAP_LINEARCOLORSTRUCT_H

#include "Struct.h"
#include "Utils/StreamUtils.h"

namespace Satisfactory3DMap {

    class LinearColorStruct : public Struct {
    public:
        LinearColorStruct(std::string struct_name, std::istream& stream) : Struct(std::move(struct_name)) {
            r_ = read<float>(stream);
            g_ = read<float>(stream);
            b_ = read<float>(stream);
            a_ = read<float>(stream);
        }

    protected:
        float r_;
        float g_;
        float b_;
        float a_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_LINEARCOLORSTRUCT_H
