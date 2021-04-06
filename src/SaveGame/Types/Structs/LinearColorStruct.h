#ifndef SATISFACTORY3DMAP_LINEARCOLORSTRUCT_H
#define SATISFACTORY3DMAP_LINEARCOLORSTRUCT_H

#include "Struct.h"

namespace Satisfactory3DMap {

    class LinearColorStruct : public Struct {
    public:
        LinearColorStruct(std::string struct_name, std::istream& stream);

        void accept(StructVisitor& v) override;

    protected:
        float r_;
        float g_;
        float b_;
        float a_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_LINEARCOLORSTRUCT_H
