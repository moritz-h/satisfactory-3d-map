#ifndef SATISFACTORY3DMAP_FLUIDBOXSTRUCT_H
#define SATISFACTORY3DMAP_FLUIDBOXSTRUCT_H

#include "Struct.h"

namespace Satisfactory3DMap {

    class FluidBoxStruct : public Struct {
    public:
        FluidBoxStruct(std::string struct_name, std::istream& stream);

        void accept(StructVisitor& v) override;

        float value() const {
            return value_;
        }

    protected:
        float value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_FLUIDBOXSTRUCT_H
