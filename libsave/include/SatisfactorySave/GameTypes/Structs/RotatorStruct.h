#pragma once

#include "../UE/Math/Rotator.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API RotatorStruct : public StructImpl<RotatorStruct, FRotator> {
    public:
        using StructImpl<RotatorStruct, FRotator>::StructImpl;
    };
} // namespace SatisfactorySave
