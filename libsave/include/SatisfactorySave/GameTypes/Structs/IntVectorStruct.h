#pragma once

#include <glm/vec3.hpp>

#include "Struct.h"

namespace SatisfactorySave {

    class IntVectorStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const glm::ivec3& value() const {
            return value_;
        }

    protected:
        glm::ivec3 value_ = glm::ivec3(0);
    };
} // namespace SatisfactorySave
