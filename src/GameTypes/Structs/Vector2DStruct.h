#pragma once

#include <glm/vec2.hpp>

#include "Struct.h"

namespace Satisfactory3DMap {

    class Vector2DStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] const glm::vec2& value() const {
            return value_;
        }

    protected:
        glm::vec2 value_ = glm::vec2(0.0f);
    };
} // namespace Satisfactory3DMap
