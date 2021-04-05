#ifndef SATISFACTORY3DMAP_VECTORSTRUCT_H
#define SATISFACTORY3DMAP_VECTORSTRUCT_H

#include <glm/vec3.hpp>

#include "Struct.h"
#include "Utils/StreamUtils.h"

namespace Satisfactory3DMap {

    class VectorStruct : public Struct {
    public:
        VectorStruct(std::string struct_name, std::istream& stream) : Struct(std::move(struct_name)) {
            value_ = read<glm::vec3>(stream);
        }

    protected:
        glm::vec3 value_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_VECTORSTRUCT_H
