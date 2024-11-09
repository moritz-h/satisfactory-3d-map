#pragma once

#include "../UE/Engine/MaterialExpressionIO.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API VectorMaterialInputStruct final
        : public StructImpl<VectorMaterialInputStruct, FVectorMaterialInput> {
    public:
        static constexpr std::string_view TypeName = "VectorMaterialInput";

        using StructImpl<VectorMaterialInputStruct, FVectorMaterialInput>::StructImpl;
    };
} // namespace SatisfactorySave
