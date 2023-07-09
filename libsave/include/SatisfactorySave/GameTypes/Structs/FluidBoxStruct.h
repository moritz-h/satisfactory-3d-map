#pragma once

#include "../UE/Satisfactory/FluidBox.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class FluidBoxStruct : public StructImpl<FluidBoxStruct, FFluidBox> {
    public:
        using StructImpl<FluidBoxStruct, FFluidBox>::StructImpl;
    };
} // namespace SatisfactorySave
