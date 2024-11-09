#pragma once

#include <cmath>

#include "../../../../IO/Archive/Archive.h"
#include "Quat.h"

namespace SatisfactorySave {

    template<typename T>
    struct TRotator {
    public:
        T Pitch{};
        T Yaw{};
        T Roll{};

        void serialize(Archive& ar) {
            ar << Pitch;
            ar << Yaw;
            ar << Roll;
        }

        // FRotator::Quaternion()
        [[nodiscard]] TQuat<T> Quaternion() const {
            static const T PI = std::atan(static_cast<T>(1.0)) * static_cast<T>(4.0);

            const T DEG_TO_RAD = PI / static_cast<T>(180.0);
            const T RADS_DIVIDED_BY_2 = DEG_TO_RAD / static_cast<T>(2.0);

            const T PitchNoWinding = std::fmod(Pitch, static_cast<T>(360.0));
            const T YawNoWinding = std::fmod(Yaw, static_cast<T>(360.0));
            const T RollNoWinding = std::fmod(Roll, static_cast<T>(360.0));

            const T SP = std::sin(PitchNoWinding * RADS_DIVIDED_BY_2);
            const T CP = std::cos(PitchNoWinding * RADS_DIVIDED_BY_2);
            const T SY = std::sin(YawNoWinding * RADS_DIVIDED_BY_2);
            const T CY = std::cos(YawNoWinding * RADS_DIVIDED_BY_2);
            const T SR = std::sin(RollNoWinding * RADS_DIVIDED_BY_2);
            const T CR = std::cos(RollNoWinding * RADS_DIVIDED_BY_2);

            TQuat<T> RotationQuat;
            RotationQuat.X = CR * SP * SY - SR * CP * CY;
            RotationQuat.Y = -CR * SP * CY - SR * CP * SY;
            RotationQuat.Z = CR * CP * SY - SR * SP * CY;
            RotationQuat.W = CR * CP * CY + SR * SP * SY;

            return RotationQuat;
        }
    };

    using FRotator = TRotator<double>;
} // namespace SatisfactorySave
