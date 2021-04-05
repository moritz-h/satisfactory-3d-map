#ifndef SATISFACTORY3DMAP_FLUIDBOXSTRUCT_H
#define SATISFACTORY3DMAP_FLUIDBOXSTRUCT_H

#include "Struct.h"
#include "Utils/StreamUtils.h"

namespace Satisfactory3DMap {

    class FluidBoxStruct : public Struct {
    public:
        FluidBoxStruct(std::string struct_name, std::istream& stream) : Struct(std::move(struct_name)) {
            value_ = read<float>(stream);
        }

    protected:
        float value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_FLUIDBOXSTRUCT_H
