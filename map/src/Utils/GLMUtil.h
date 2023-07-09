#pragma once

#include <glm/glm.hpp>

#include "SatisfactorySave/GameTypes/UE/Math/Quat.h"
#include "SatisfactorySave/GameTypes/UE/Math/Vector.h"

namespace Satisfactory3DMap {

    static inline glm::vec3 glmCast(const SatisfactorySave::FVector& vec) {
        return {vec.X, vec.Y, vec.Z};
    }

    static inline glm::quat glmCast(const SatisfactorySave::FQuat& quat) {
        return glm::quat(quat.W, quat.X, quat.Y, quat.Z);
    }
} // namespace Satisfactory3DMap
