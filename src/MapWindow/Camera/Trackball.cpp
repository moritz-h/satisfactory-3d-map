#include "Trackball.h"

#include <glm/glm.hpp>

Satisfactory3DMap::Trackball::Trackball() : radius_(0.8f), q_(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {}

glm::mat4 Satisfactory3DMap::Trackball::getRot() const {
    return glm::mat4_cast(q_);
}

void Satisfactory3DMap::Trackball::reset() {
    q_ = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}

void Satisfactory3DMap::Trackball::track(float p1x, float p1y, float p2x, float p2y) {
    if (p1x == p2x && p1y == p2y) {
        return;
    }

    glm::vec3 p1 = mapToSphere(p1x, p1y);
    glm::vec3 p2 = mapToSphere(p2x, p2y);
    glm::vec3 rotAxis = glm::cross(p1, p2);

    float t = glm::length(p1 - p2) / (2.0f * radius_);
    if (t > 1.0f) {
        t = 1.0f;
    }
    if (t < -1.0f) {
        t = -1.0f;
    }

    float phi = 2.0f * glm::asin(t);

    rotAxis = glm::normalize(rotAxis);
    glm::quat rq = glm::quat(glm::cos(phi / 2.0f), rotAxis * glm::sin(phi / 2.0f));

    q_ = rq * q_;
}

glm::vec3 Satisfactory3DMap::Trackball::mapToSphere(float x, float y) const {
    float z;
    float d = glm::sqrt(x * x + y * y);
    if (d < radius_ / glm::sqrt(2.0f)) {
        z = glm::sqrt(radius_ * radius_ - d * d);
    } else {
        z = 0.5f * radius_ * radius_ / d;
    }
    return glm::vec3(x, y, z);
}
