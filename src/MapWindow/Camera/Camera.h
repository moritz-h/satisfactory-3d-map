#ifndef SATISFACTORY3DMAP_CAMERA_H
#define SATISFACTORY3DMAP_CAMERA_H

#include <glm/fwd.hpp>

#include "Trackball.h"

namespace Satisfactory3DMap {
    class Camera {
    public:
        enum class MouseControlMode {
            None = 0,
            Left = 1,
            Middle = 2,
            Right = 3,
        };

        explicit Camera(float dolly = 5.0f);
        ~Camera() = default;

        void mouseMoveControl(MouseControlMode mode, double oldX, double oldY, double newX, double newY);
        void mouseScrollControl(double xoffset, double yoffset);

        [[nodiscard]] const glm::mat4& viewMx() const {
            return viewMx_;
        };

        void rotate(float p1x, float p1y, float p2x, float p2y);

        void moveDolly(float d);

        void reset();

    private:
        void updateMx();

        float defaultDolly_;

        Trackball t_;
        float dolly_;
        float panx_;
        float pany_;
        glm::mat4 viewMx_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_CAMERA_H
