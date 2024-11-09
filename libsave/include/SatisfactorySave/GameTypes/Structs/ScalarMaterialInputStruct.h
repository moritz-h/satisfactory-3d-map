#pragma once

#include "../UE/Engine/MaterialExpressionIO.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ScalarMaterialInputStruct final
        : public StructImpl<ScalarMaterialInputStruct, FScalarMaterialInput> {
    public:
        static constexpr std::string_view TypeName = "ScalarMaterialInput";

        using StructImpl<ScalarMaterialInputStruct, FScalarMaterialInput>::StructImpl;
    };
} // namespace SatisfactorySave
