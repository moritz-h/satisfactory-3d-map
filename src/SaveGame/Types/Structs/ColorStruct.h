#ifndef SATISFACTORY3DMAP_COLORSTRUCT_H
#define SATISFACTORY3DMAP_COLORSTRUCT_H

#include "Struct.h"

namespace Satisfactory3DMap {

    class ColorStruct : public Struct {
    public:
        ColorStruct(std::string struct_name, std::istream& stream);

        void accept(StructVisitor& v) override;

    protected:
        int8_t b_;
        int8_t g_;
        int8_t r_;
        int8_t a_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_COLORSTRUCT_H
