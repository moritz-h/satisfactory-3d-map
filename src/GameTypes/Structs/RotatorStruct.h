#pragma once

#include <glm/gtc/quaternion.hpp>

#include "Struct.h"

namespace Satisfactory3DMap {

    class RotatorStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] float pitch() const {
            return pitch_;
        }

        [[nodiscard]] float yaw() const {
            return yaw_;
        }

        [[nodiscard]] float roll() const {
            return roll_;
        }

        [[nodiscard]] glm::quat quaternion() const;

    protected:
        float pitch_ = 0.0f;
        float yaw_ = 0.0f;
        float roll_ = 0.0f;
    };
} // namespace Satisfactory3DMap
