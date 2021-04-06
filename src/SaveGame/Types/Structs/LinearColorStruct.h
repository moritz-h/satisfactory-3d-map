#ifndef SATISFACTORY3DMAP_LINEARCOLORSTRUCT_H
#define SATISFACTORY3DMAP_LINEARCOLORSTRUCT_H

#include "Struct.h"

namespace Satisfactory3DMap {

    class LinearColorStruct : public Struct {
    public:
        LinearColorStruct(std::string struct_name, std::istream& stream);

        void accept(StructVisitor& v) override;

        float r() const {
            return r_;
        }

        float g() const {
            return g_;
        }

        float b() const {
            return b_;
        }

        float a() const {
            return a_;
        }

    protected:
        float r_;
        float g_;
        float b_;
        float a_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_LINEARCOLORSTRUCT_H
