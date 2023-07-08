#include "GameTypes/Structs/RotatorStruct.h"

#include <cmath>

#include "GameTypes/Structs/StructVisitor.h"

void SatisfactorySave::RotatorStruct::serialize(Archive& ar) {
    ar << pitch_;
    ar << yaw_;
    ar << roll_;
}

void SatisfactorySave::RotatorStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}

// FRotator::Quaternion()
glm::quat SatisfactorySave::RotatorStruct::quaternion() const {
    static const float PI = std::atan(1.0f) * 4.0f;

    const float DEG_TO_RAD = PI / 180.0f;
    const float RADS_DIVIDED_BY_2 = DEG_TO_RAD / 2.f;

    const float PitchNoWinding = std::fmod(static_cast<float>(pitch_), 360.0f);
    const float YawNoWinding = std::fmod(static_cast<float>(yaw_), 360.0f);
    const float RollNoWinding = std::fmod(static_cast<float>(roll_), 360.0f);

    float SP = std::sin(PitchNoWinding * RADS_DIVIDED_BY_2);
    float CP = std::cos(PitchNoWinding * RADS_DIVIDED_BY_2);
    float SY = std::sin(YawNoWinding * RADS_DIVIDED_BY_2);
    float CY = std::cos(YawNoWinding * RADS_DIVIDED_BY_2);
    float SR = std::sin(RollNoWinding * RADS_DIVIDED_BY_2);
    float CR = std::cos(RollNoWinding * RADS_DIVIDED_BY_2);

    glm::quat RotationQuat;
    RotationQuat.x = CR * SP * SY - SR * CP * CY;
    RotationQuat.y = -CR * SP * CY - SR * CP * SY;
    RotationQuat.z = CR * CP * SY - SR * SP * CY;
    RotationQuat.w = CR * CP * CY + SR * SP * SY;

    return RotationQuat;
}