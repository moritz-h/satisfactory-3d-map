#pragma once

#include <glm/vec3.hpp>

#include "Base/Struct.h"

namespace SatisfactorySave {

    class VectorStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const glm::dvec3& value() const {
            return value_;
        }

    protected:
        glm::dvec3 value_ = glm::dvec3(0.0f);
    };
} // namespace SatisfactorySave
