#include "Camera3D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

Satisfactory3DMap::Camera3D::Camera3D() {
    pos_ = glm::vec3(0.0f, -4000.0f, 1000.0f);
    yaw_ = 0.0f;
    pitch_ = 0.0f;
    updateMx();
}

void Satisfactory3DMap::Camera3D::mouseMoveControl(
    MouseControlMode mode, double oldX, double oldY, double newX, double newY) {
    if (mode == MouseControlMode::Left) {
        yaw_ += static_cast<float>(oldX - newX);
        pitch_ += static_cast<float>(newY - oldY);
        const float pi2 = 2.0f * glm::pi<float>();
        while (yaw_ < 0.0f) {
            yaw_ += pi2;
        }
        while (yaw_ >= pi2) {
            yaw_ -= pi2;
        }
        pitch_ = glm::clamp(pitch_, -0.5f * glm::pi<float>(), 0.5f * glm::pi<float>());
    } else if (mode == MouseControlMode::Right) {
        pos_ += front_ * 1000.0f * static_cast<float>(newY - oldY);
    } else if (mode == MouseControlMode::Middle) {
    }
    updateMx();
}

void Satisfactory3DMap::Camera3D::mouseScrollControl([[maybe_unused]] double xoffset, double yoffset) {}

void Satisfactory3DMap::Camera3D::reset() {
    pos_ = glm::vec3(0.0f, -4000.0f, 1000.0f);
    yaw_ = 0.0f;
    pitch_ = 0.0f;
    updateMx();
}

void Satisfactory3DMap::Camera3D::updateMx() {
    glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 front = glm::vec3(0.0f, 1.0f, 0.0f);
    front = glm::rotate(front, yaw_, up);
    glm::vec3 right = glm::cross(front, up);
    front = glm::rotate(front, pitch_, right);
    up = glm::cross(right, front);
    front_ = front;
    viewMx_ = glm::lookAt(pos_, pos_ + front, up);
}
