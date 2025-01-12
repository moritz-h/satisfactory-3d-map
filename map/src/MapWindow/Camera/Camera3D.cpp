#include "Camera3D.h"

#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <imgui.h>

Satisfactory3DMap::Camera3D::Camera3D()
    : pos_(0.0f, 4000.0f, 1000.0f),
      yaw_(0.0f),
      pitch_(0.0f),
      front_(0.0f),
      left_(0.0f) {
    updateMx();
}

void Satisfactory3DMap::Camera3D::keyPressedControl(KeyControl key, double deltaT) {
    const float factor = 2000.0f;

    const glm::vec3 up(0.0f, 0.0f, 1.0f);
    switch (key) {
        case KeyControl::Forward:
            pos_ += front_ * factor * static_cast<float>(deltaT);
            break;
        case KeyControl::Backward:
            pos_ -= front_ * factor * static_cast<float>(deltaT);
            break;
        case KeyControl::Left:
            pos_ += left_ * factor * static_cast<float>(deltaT);
            break;
        case KeyControl::Right:
            pos_ -= left_ * factor * static_cast<float>(deltaT);
            break;
        case KeyControl::Up:
            pos_ += up * factor * static_cast<float>(deltaT);
            break;
        case KeyControl::Down:
            pos_ -= up * factor * static_cast<float>(deltaT);
            break;
        case KeyControl::TurnLeft:
            yaw_ -= static_cast<float>(deltaT);
            clampPitchYaw();
            break;
        case KeyControl::TurnRight:
            yaw_ += static_cast<float>(deltaT);
            clampPitchYaw();
            break;
        case KeyControl::TurnUp:
            pitch_ += static_cast<float>(deltaT);
            clampPitchYaw();
            break;
        case KeyControl::TurnDown:
            pitch_ -= static_cast<float>(deltaT);
            clampPitchYaw();
            break;
        default:
            break;
    }

    updateMx();
}

void Satisfactory3DMap::Camera3D::mouseMoveControl(MouseControlMode mode, glm::dvec2 oldPos, glm::dvec2 newPos,
    [[maybe_unused]] glm::ivec2 windowSize) {
    if (mode == MouseControlMode::Left) {
        yaw_ += 0.002f * static_cast<float>(newPos.x - oldPos.x);
        pitch_ += 0.002f * static_cast<float>(oldPos.y - newPos.y);
        clampPitchYaw();
    } else if (mode == MouseControlMode::Right) {
        pos_ += front_ * 2.0f * static_cast<float>(oldPos.y - newPos.y);
    } else if (mode == MouseControlMode::Middle) {
    }
    updateMx();
}

void Satisfactory3DMap::Camera3D::mouseScrollControl([[maybe_unused]] glm::dvec2 offset) {}

void Satisfactory3DMap::Camera3D::reset() {
    pos_ = glm::vec3(0.0f, 4000.0f, 1000.0f);
    yaw_ = 0.0f;
    pitch_ = 0.0f;
    updateMx();
}

void Satisfactory3DMap::Camera3D::renderGui() {
    float yaw_deg = glm::degrees(yaw_);
    float pitch_deg = glm::degrees(pitch_);
    bool changed = false;
    changed |= ImGui::DragFloat3("Pos", glm::value_ptr(pos_), 1.0f, 0.0f, 0.0f, "%.1f");
    changed |= ImGui::DragFloat("Yaw", &yaw_deg, 0.25f, 0.0f, 0.0f, "%.2f deg");
    changed |= ImGui::DragFloat("Pitch", &pitch_deg, 0.25f, -90.0f, 90.0f, "%.2f deg");
    if (changed) {
        yaw_ = glm::radians(yaw_deg);
        pitch_ = glm::radians(pitch_deg);
        clampPitchYaw();
        updateMx();
    }
}

void Satisfactory3DMap::Camera3D::clampPitchYaw() {
    constexpr float pi2 = 2.0f * glm::pi<float>();
    while (yaw_ < 0.0f) {
        yaw_ += pi2;
    }
    while (yaw_ >= pi2) {
        yaw_ -= pi2;
    }
    pitch_ = glm::clamp(pitch_, -0.5f * glm::pi<float>(), 0.5f * glm::pi<float>());
}

void Satisfactory3DMap::Camera3D::updateMx() {
    glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 front = glm::vec3(0.0f, -1.0f, 0.0f);
    front = glm::rotate(front, yaw_, up);
    glm::vec3 left = glm::cross(front, up);
    front = glm::rotate(front, pitch_, left);
    up = glm::cross(left, front);
    front_ = front;
    left_ = left;
    viewMx_ = glm::lookAt(pos_, pos_ + front, up);
}
