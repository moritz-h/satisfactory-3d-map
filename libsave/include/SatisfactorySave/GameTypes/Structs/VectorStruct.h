#pragma once

#include "../UE/Math/Vector.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class VectorStruct : public StructImpl<VectorStruct, FVector> {
    public:
        using StructImpl<VectorStruct, FVector>::StructImpl;
    };
} // namespace SatisfactorySave
