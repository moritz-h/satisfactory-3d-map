#pragma once

#include <glm/gtc/quaternion.hpp>

#include "Base/Struct.h"

namespace SatisfactorySave {

    class RotatorStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] double pitch() const {
            return pitch_;
        }

        [[nodiscard]] double yaw() const {
            return yaw_;
        }

        [[nodiscard]] double roll() const {
            return roll_;
        }

        [[nodiscard]] glm::quat quaternion() const;

    protected:
        double pitch_ = 0.0f;
        double yaw_ = 0.0f;
        double roll_ = 0.0f;
    };
} // namespace SatisfactorySave
