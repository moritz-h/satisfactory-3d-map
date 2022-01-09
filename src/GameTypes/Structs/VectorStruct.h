#ifndef SATISFACTORY3DMAP_VECTORSTRUCT_H
#define SATISFACTORY3DMAP_VECTORSTRUCT_H

#include <glm/vec3.hpp>

#include "Struct.h"

namespace Satisfactory3DMap {

    class VectorStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const glm::vec3& value() const {
            return value_;
        }

    protected:
        glm::vec3 value_ = glm::vec3(0.0f);
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_VECTORSTRUCT_H
