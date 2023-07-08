#pragma once

#include "AbstractCamera.h"

namespace Satisfactory3DMap {
    class Camera3D : public AbstractCamera {
    public:
        Camera3D();
        ~Camera3D() override = default;

        void keyPressedControl(KeyControl key, double deltaT) override;
        void mouseMoveControl(MouseControlMode mode, glm::dvec2 oldPos, glm::dvec2 newPos,
            glm::ivec2 windowSize) override;
        void mouseScrollControl(glm::dvec2 offset) override;

        void reset() override;

    private:
        void clampPitchYaw();
        void updateMx();

        glm::vec3 pos_;
        float yaw_;
        float pitch_;
        glm::vec3 front_;
        glm::vec3 right_;
    };
} // namespace Satisfactory3DMap
