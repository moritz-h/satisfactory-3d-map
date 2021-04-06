#ifndef SATISFACTORY3DMAP_BOXSTRUCT_H
#define SATISFACTORY3DMAP_BOXSTRUCT_H

#include <glm/vec3.hpp>

#include "Struct.h"

namespace Satisfactory3DMap {

    class BoxStruct : public Struct {
    public:
        BoxStruct(std::string struct_name, std::istream& stream);

        void accept(StructVisitor& v) override;

    protected:
        glm::vec3 min_;
        glm::vec3 max_;
        uint8_t is_valid_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BOXSTRUCT_H
