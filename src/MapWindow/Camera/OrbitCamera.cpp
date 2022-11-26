#include "OrbitCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Satisfactory3DMap::OrbitCamera::OrbitCamera(float dolly)
    : defaultDolly_(dolly),
      dolly_(dolly),
      panx_(0.0f),
      pany_(0.0f) {
    updateMx();
}

void Satisfactory3DMap::OrbitCamera::keyPressedControl([[maybe_unused]] KeyControl key,
    [[maybe_unused]] double deltaT) {}

void Satisfactory3DMap::OrbitCamera::mouseMoveControl(MouseControlMode mode, glm::dvec2 oldPos, glm::dvec2 newPos,
    glm::ivec2 windowSize) {

    // Transform from window coords in px with (0,0) at top left to right-handed coordinate system in [-1, 1] range.
    float oldX = 2.0f * static_cast<float>(oldPos.x) / static_cast<float>(windowSize.x) - 1.0f;
    float oldY = 1.0f - 2.0f * static_cast<float>(oldPos.y) / static_cast<float>(windowSize.y);
    float newX = 2.0f * static_cast<float>(newPos.x) / static_cast<float>(windowSize.x) - 1.0f;
    float newY = 1.0f - 2.0f * static_cast<float>(newPos.y) / static_cast<float>(windowSize.y);

    if (mode == MouseControlMode::Left) {
        rotate(oldX, oldY, newX, newY);
    } else if (mode == MouseControlMode::Right) {
        moveDolly(2000.0f * (oldY - newY));
    } else if (mode == MouseControlMode::Middle) {
        panx_ += 600.0f * (newX - oldX);
        pany_ += 600.0f * (newY - oldY);
        updateMx();
    }
}

void Satisfactory3DMap::OrbitCamera::mouseScrollControl(glm::dvec2 offset) {
    moveDolly(-10.0f * static_cast<float>(offset.y));
}

void Satisfactory3DMap::OrbitCamera::rotate(float p1x, float p1y, float p2x, float p2y) {
    t_.track(p1x, p1y, p2x, p2y);
    updateMx();
}

void Satisfactory3DMap::OrbitCamera::moveDolly(float d) {
    dolly_ += d;
    if (dolly_ < 0.0f) {
        dolly_ = 0.0f;
    }
    updateMx();
}

void Satisfactory3DMap::OrbitCamera::reset() {
    t_.reset();
    dolly_ = defaultDolly_;
    panx_ = 0.0f;
    pany_ = 0.0f;
    updateMx();
}

void Satisfactory3DMap::OrbitCamera::updateMx() {
    viewMx_ = glm::translate(glm::mat4(1.0f), glm::vec3(panx_, pany_, -dolly_));
    viewMx_ *= t_.getRot();
}
