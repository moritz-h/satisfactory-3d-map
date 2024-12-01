#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "SatisfactorySave/GameTypes/UE/Core/Math/Quat.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Transform.h"
#include "SatisfactorySave/GameTypes/UE/Core/Math/Vector.h"

namespace Satisfactory3DMap {

    template<typename T>
    static inline glm::vec3 glmCast(const SatisfactorySave::TVector<T>& vec) {
        return {vec.X, vec.Y, vec.Z};
    }

    template<typename T>
    static inline glm::quat glmCast(const SatisfactorySave::TQuat<T>& quat) {
        return glm::quat(quat.W, quat.X, quat.Y, quat.Z);
    }

    template<typename T>
    static inline glm::mat4 glmCast(const SatisfactorySave::TTransform<T>& t) {
        // Transform translation from [cm] to [m]
        const auto translation = glm::translate(glm::mat4(1.0f), glmCast(t.Translation) * glm::vec3(0.01f));
        const auto rotation = glm::mat4_cast(glmCast(t.Rotation));
        const auto scale = glm::scale(glm::mat4(1.0f), glmCast(t.Scale3D));
        return translation * rotation * scale;
    }

    static inline SatisfactorySave::TVector<float> ueVec3fCast(const glm::vec3& vec) {
        return {vec.x, vec.y, vec.z};
    }

    static inline SatisfactorySave::TQuat<float> ueQuatfCast(const glm::quat& quat) {
        return {quat.x, quat.y, quat.z, quat.w};
    }
} // namespace Satisfactory3DMap
