#pragma once

#include "../UE/Math/IntPoint.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class IntPointStruct : public StructImpl<IntPointStruct, FIntPoint> {
    public:
        using StructImpl<IntPointStruct, FIntPoint>::StructImpl;
    };
} // namespace SatisfactorySave
