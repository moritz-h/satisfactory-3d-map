#pragma once

#include "../FactoryGame/FGFluidIntegrantInterface.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API FluidBoxStruct final : public StructImpl<FluidBoxStruct, FFluidBox> {
    public:
        static constexpr std::string_view TypeName = "FluidBox";

        using StructImpl<FluidBoxStruct, FFluidBox>::StructImpl;
    };
} // namespace SatisfactorySave
