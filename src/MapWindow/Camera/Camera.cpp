#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Satisfactory3DMap::Camera::Camera(float dolly)
    : defaultDolly_(dolly), dolly_(dolly), panx_(0.0f), pany_(0.0f), viewMx_(glm::mat4(1.0)) {
    updateMx();
}

void Satisfactory3DMap::Camera::mouseMoveControl(
    MouseControlMode mode, double oldX, double oldY, double newX, double newY) {
    if (mode == MouseControlMode::Left) {
        rotate(static_cast<float>(oldX), static_cast<float>(oldY), static_cast<float>(newX), static_cast<float>(newY));
    } else if (mode == MouseControlMode::Right) {
        moveDolly(2000.0f * static_cast<float>(oldY - newY));
    } else if (mode == MouseControlMode::Middle) {
        panx_ += 600.0f * static_cast<float>(newX - oldX);
        pany_ += 600.0f * static_cast<float>(newY - oldY);
        updateMx();
    }
}

void Satisfactory3DMap::Camera::mouseScrollControl([[maybe_unused]] double xoffset, double yoffset) {
    moveDolly(-10.0f * static_cast<float>(yoffset));
}

void Satisfactory3DMap::Camera::rotate(float p1x, float p1y, float p2x, float p2y) {
    t_.track(p1x, p1y, p2x, p2y);
    updateMx();
}

void Satisfactory3DMap::Camera::moveDolly(float d) {
    dolly_ += d;
    if (dolly_ < 0.0f) {
        dolly_ = 0.0f;
    }
    updateMx();
}

void Satisfactory3DMap::Camera::reset() {
    t_.reset();
    dolly_ = defaultDolly_;
    panx_ = 0.0f;
    pany_ = 0.0f;
}

void Satisfactory3DMap::Camera::updateMx() {
    viewMx_ = glm::translate(glm::mat4(1.0f), glm::vec3(panx_, pany_, -dolly_));
    viewMx_ *= t_.getRot();
}
