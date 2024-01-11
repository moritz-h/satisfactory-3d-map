#pragma once

#include "../UE/Materials/MaterialInput.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API VectorMaterialInputStruct
        : public StructImpl<VectorMaterialInputStruct, FVectorMaterialInput> {
    public:
        using StructImpl<VectorMaterialInputStruct, FVectorMaterialInput>::StructImpl;
    };
} // namespace SatisfactorySave
