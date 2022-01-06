#ifndef SATISFACTORY3DMAP_QUATSTRUCT_H
#define SATISFACTORY3DMAP_QUATSTRUCT_H

#include "Struct.h"

namespace Satisfactory3DMap {

    class QuatStruct : public Struct {
    public:
        QuatStruct(std::string struct_name, std::istream& stream);

        void serialize(std::ostream& stream) const override;

        void accept(StructVisitor& v) override;

        float x() const {
            return x_;
        }

        float y() const {
            return y_;
        }

        float z() const {
            return z_;
        }

        float w() const {
            return w_;
        }

    protected:
        float x_;
        float y_;
        float z_;
        float w_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_QUATSTRUCT_H
