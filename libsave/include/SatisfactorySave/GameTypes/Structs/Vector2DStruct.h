#pragma once

#include <glm/vec2.hpp>

#include "Base/Struct.h"

namespace SatisfactorySave {

    class Vector2DStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const glm::dvec2& value() const {
            return value_;
        }

    protected:
        glm::dvec2 value_ = glm::vec2(0.0f);
    };
} // namespace SatisfactorySave
