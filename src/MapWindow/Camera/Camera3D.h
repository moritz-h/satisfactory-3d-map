#ifndef SATISFACTORY3DMAP_CAMERA3D_H
#define SATISFACTORY3DMAP_CAMERA3D_H

#include "AbstractCamera.h"

namespace Satisfactory3DMap {
    class Camera3D : public AbstractCamera {
    public:
        Camera3D();
        ~Camera3D() override = default;

        void mouseMoveControl(MouseControlMode mode, double oldX, double oldY, double newX, double newY) override;
        void mouseScrollControl(double xoffset, double yoffset) override;

        void reset() override;

    private:
        void updateMx();

        glm::vec3 pos_;
        float yaw_;
        float pitch_;
        glm::vec3 front_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_CAMERA3D_H
