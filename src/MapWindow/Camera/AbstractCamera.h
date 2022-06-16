#pragma once

#include <glm/glm.hpp>

namespace Satisfactory3DMap {
    class AbstractCamera {
    public:
        enum class KeyControl {
            None = 0,
            Forward = 1,
            Backward = 2,
            Left = 3,
            Right = 4,
            Up = 5,
            Down = 6,
            TurnLeft = 7,
            TurnRight = 8,
            TurnUp = 9,
            TurnDown = 10,
        };

        enum class MouseControlMode {
            None = 0,
            Left = 1,
            Middle = 2,
            Right = 3,
        };

        AbstractCamera() : viewMx_(glm::mat4(1.0f)) {}
        virtual ~AbstractCamera() = default;

        virtual void keyPressedControl(KeyControl key, double deltaT) = 0;
        virtual void mouseMoveControl(MouseControlMode mode, double oldX, double oldY, double newX, double newY) = 0;
        virtual void mouseScrollControl(double xoffset, double yoffset) = 0;

        virtual void reset() = 0;

        [[nodiscard]] const glm::mat4& viewMx() const {
            return viewMx_;
        };

    protected:
        glm::mat4 viewMx_;
    };
} // namespace Satisfactory3DMap
