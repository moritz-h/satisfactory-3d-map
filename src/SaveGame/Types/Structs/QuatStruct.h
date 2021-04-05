#ifndef SATISFACTORY3DMAP_QUATSTRUCT_H
#define SATISFACTORY3DMAP_QUATSTRUCT_H

#include "Struct.h"
#include "Utils/StreamUtils.h"

namespace Satisfactory3DMap {

    class QuatStruct : public Struct {
    public:
        QuatStruct(std::string struct_name, std::istream& stream) : Struct(std::move(struct_name)) {
            x_ = read<float>(stream);
            y_ = read<float>(stream);
            z_ = read<float>(stream);
            w_ = read<float>(stream);
        }

    protected:
        float x_;
        float y_;
        float z_;
        float w_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_QUATSTRUCT_H
