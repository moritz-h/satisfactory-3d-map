#pragma once

#include "../../Core/Math/Quat.h"
#include "../../Core/Math/Vector.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FRigidBodyState {
    public:
        FVector Position;
        FQuat Quaternion;
        FVector LinVel;
        FVector AngVel;
        uint8_t Flags = 0;
    };

} // namespace SatisfactorySave
