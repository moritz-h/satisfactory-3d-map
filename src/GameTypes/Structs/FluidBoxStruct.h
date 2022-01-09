#ifndef SATISFACTORY3DMAP_FLUIDBOXSTRUCT_H
#define SATISFACTORY3DMAP_FLUIDBOXSTRUCT_H

#include "Struct.h"

namespace Satisfactory3DMap {

    class FluidBoxStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] float value() const {
            return value_;
        }

    protected:
        float value_ = 0.0f;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_FLUIDBOXSTRUCT_H
