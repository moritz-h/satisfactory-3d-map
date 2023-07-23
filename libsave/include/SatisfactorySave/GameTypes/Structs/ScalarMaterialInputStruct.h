#pragma once

#include "../UE/Materials/MaterialInput.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class ScalarMaterialInputStruct : public StructImpl<ScalarMaterialInputStruct, FScalarMaterialInput> {
    public:
        using StructImpl<ScalarMaterialInputStruct, FScalarMaterialInput>::StructImpl;
    };
} // namespace SatisfactorySave
