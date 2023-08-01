#pragma once

#include "AbstractCamera.h"
#include "Trackball.h"

namespace Satisfactory3DMap {
    class OrbitCamera : public AbstractCamera {
    public:
        explicit OrbitCamera(float dolly = 8000.0f);
        ~OrbitCamera() override = default;

        void keyPressedControl(KeyControl key, double deltaT) override;
        void mouseMoveControl(MouseControlMode mode, glm::dvec2 oldPos, glm::dvec2 newPos,
            glm::ivec2 windowSize) override;
        void mouseScrollControl(glm::dvec2 offset) override;

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
