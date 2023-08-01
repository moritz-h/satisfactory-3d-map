#pragma once

#include "../UE/Math/Rotator.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class RotatorStruct : public StructImpl<RotatorStruct, FRotator> {
    public:
        using StructImpl<RotatorStruct, FRotator>::StructImpl;
    };
} // namespace SatisfactorySave
