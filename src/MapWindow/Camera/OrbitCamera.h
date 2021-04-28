#ifndef SATISFACTORY3DMAP_ORBITCAMERA_H
#define SATISFACTORY3DMAP_ORBITCAMERA_H

#include "AbstractCamera.h"
#include "Trackball.h"

namespace Satisfactory3DMap {
    class OrbitCamera : public AbstractCamera {
    public:
        explicit OrbitCamera(float dolly = 5.0f);
        ~OrbitCamera() override = default;

        void mouseMoveControl(MouseControlMode mode, double oldX, double oldY, double newX, double newY) override;
        void mouseScrollControl(double xoffset, double yoffset) override;

        void rotate(float p1x, float p1y, float p2x, float p2y);

        void moveDolly(float d);

        void reset() override;

    private:
        void updateMx();

        float defaultDolly_;

        Trackball t_;
        float dolly_;
        float panx_;
        float pany_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ORBITCAMERA_H
