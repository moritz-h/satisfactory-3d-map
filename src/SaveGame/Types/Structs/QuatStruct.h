#ifndef SATISFACTORY3DMAP_QUATSTRUCT_H
#define SATISFACTORY3DMAP_QUATSTRUCT_H

#include "Struct.h"

namespace Satisfactory3DMap {

    class QuatStruct : public Struct {
    public:
        QuatStruct(std::string struct_name, std::istream& stream);

        void accept(StructVisitor& v) override;

    protected:
        float x_;
        float y_;
        float z_;
        float w_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_QUATSTRUCT_H
