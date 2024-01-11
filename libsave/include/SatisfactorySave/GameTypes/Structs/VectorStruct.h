#pragma once

#include "../UE/Math/Vector.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API VectorStruct : public StructImpl<VectorStruct, FVector> {
    public:
        using StructImpl<VectorStruct, FVector>::StructImpl;
    };
} // namespace SatisfactorySave
