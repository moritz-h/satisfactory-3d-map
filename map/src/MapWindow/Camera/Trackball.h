#pragma once

#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Satisfactory3DMap {
    class Trackball {
    public:
        Trackball();

        glm::quat& getQ() {
            return q_;
        }

        [[nodiscard]] const glm::quat& getQ() const {
            return q_;
        }

        [[nodiscard]] glm::mat4 getRot() const;

        void reset();

        void track(float p1x, float p1y, float p2x, float p2y);

    private:
        [[nodiscard]] glm::vec3 mapToSphere(float x, float y) const;

        const float radius_;
        glm::quat q_;
    };
} // namespace Satisfactory3DMap
