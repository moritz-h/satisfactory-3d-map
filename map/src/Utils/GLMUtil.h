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
    inline glm::vec3 glmCast(const SatisfactorySave::TVector<T>& vec) {
        return {static_cast<float>(vec.X), static_cast<float>(vec.Y), static_cast<float>(vec.Z)};
    }

    template<typename T>
    inline glm::quat glmCast(const SatisfactorySave::TQuat<T>& quat) {
        return glm::quat(static_cast<float>(quat.W), static_cast<float>(quat.X), static_cast<float>(quat.Y),
            static_cast<float>(quat.Z));
    }

    template<typename T>
    inline glm::mat4 glmCast(const SatisfactorySave::TTransform<T>& t) {
        // Transform translation from [cm] to [m]
        const auto translation = glm::translate(glm::mat4(1.0f), glmCast(t.Translation) * glm::vec3(0.01f));
        const auto rotation = glm::mat4_cast(glmCast(t.Rotation));
        const auto scale = glm::scale(glm::mat4(1.0f), glmCast(t.Scale3D));
        return translation * rotation * scale;
    }

    template<typename T>
    inline SatisfactorySave::TVector<T> ueVectorCast(const glm::vec3& vec) {
        return {static_cast<T>(vec.x), static_cast<T>(vec.y), static_cast<T>(vec.z)};
    }

    template<typename T>
    inline SatisfactorySave::TQuat<T> ueQuatCast(const glm::quat& quat) {
        return {static_cast<T>(quat.x), static_cast<T>(quat.y), static_cast<T>(quat.z), static_cast<T>(quat.w)};
    }
} // namespace Satisfactory3DMap
