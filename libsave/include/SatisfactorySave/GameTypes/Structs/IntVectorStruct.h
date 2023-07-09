#pragma once

#include "../UE/Math/IntVector.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class IntVectorStruct : public StructImpl<IntVectorStruct, FIntVector> {
    public:
        using StructImpl<IntVectorStruct, FIntVector>::StructImpl;
    };
} // namespace SatisfactorySave
