#ifndef SATISFACTORY3DMAP_INTPOINTSTRUCT_H
#define SATISFACTORY3DMAP_INTPOINTSTRUCT_H

#include "Struct.h"

namespace Satisfactory3DMap {

    class IntPointStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] int32_t x() const {
            return x_;
        }

        [[nodiscard]] int32_t y() const {
            return y_;
        }

    protected:
        int32_t x_ = 0;
        int32_t y_ = 0;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_INTPOINTSTRUCT_H
