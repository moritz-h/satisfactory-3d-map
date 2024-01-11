#pragma once

#include "../UE/Math/Quat.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API QuatStruct : public StructImpl<QuatStruct, FQuat> {
    public:
        using StructImpl<QuatStruct, FQuat>::StructImpl;
    };
} // namespace SatisfactorySave
