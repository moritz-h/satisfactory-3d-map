#pragma once

#include <cmath>

#include "../../../IO/Archive/Archive.h"
#include "Quat.h"

namespace SatisfactorySave {

    // FRotator
    struct FRotator {
    public:
        double Pitch = 0.0;
        double Yaw = 0.0;
        double Roll = 0.0;

        void serialize(Archive& ar) {
            ar << Pitch;
            ar << Yaw;
            ar << Roll;
        }

        // FRotator::Quaternion()
        FQuat Quaternion() const {
            static const double PI = std::atan(1.0) * 4.0;

            const double DEG_TO_RAD = PI / 180.0;
            const double RADS_DIVIDED_BY_2 = DEG_TO_RAD / 2.0;

            const double PitchNoWinding = std::fmod(Pitch, 360.0);
            const double YawNoWinding = std::fmod(Yaw, 360.0);
            const double RollNoWinding = std::fmod(Roll, 360.0);

            double SP = std::sin(PitchNoWinding * RADS_DIVIDED_BY_2);
            double CP = std::cos(PitchNoWinding * RADS_DIVIDED_BY_2);
            double SY = std::sin(YawNoWinding * RADS_DIVIDED_BY_2);
            double CY = std::cos(YawNoWinding * RADS_DIVIDED_BY_2);
            double SR = std::sin(RollNoWinding * RADS_DIVIDED_BY_2);
            double CR = std::cos(RollNoWinding * RADS_DIVIDED_BY_2);

            FQuat RotationQuat;
            RotationQuat.X = CR * SP * SY - SR * CP * CY;
            RotationQuat.Y = -CR * SP * CY - SR * CP * SY;
            RotationQuat.Z = CR * CP * SY - SR * SP * CY;
            RotationQuat.W = CR * CP * CY + SR * SP * SY;

            return RotationQuat;
        }
    };
} // namespace SatisfactorySave
