#ifndef SATISFACTORY3DMAP_BOXSTRUCT_H
#define SATISFACTORY3DMAP_BOXSTRUCT_H

#include <glm/vec3.hpp>

#include "Struct.h"
#include "Utils/StreamUtils.h"

namespace Satisfactory3DMap {

    class BoxStruct : public Struct {
    public:
        BoxStruct(std::string struct_name, std::istream& stream) : Struct(std::move(struct_name)) {
            min_ = read<glm::vec3>(stream);
            max_ = read<glm::vec3>(stream);
            is_valid_ = read<uint8_t>(stream);
        }

    protected:
        glm::vec3 min_;
        glm::vec3 max_;
        uint8_t is_valid_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BOXSTRUCT_H
