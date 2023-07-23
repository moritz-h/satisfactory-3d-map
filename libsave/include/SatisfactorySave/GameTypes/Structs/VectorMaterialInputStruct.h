#pragma once

#include "../UE/Materials/MaterialInput.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class VectorMaterialInputStruct : public StructImpl<VectorMaterialInputStruct, FVectorMaterialInput> {
    public:
        using StructImpl<VectorMaterialInputStruct, FVectorMaterialInput>::StructImpl;
    };
} // namespace SatisfactorySave
