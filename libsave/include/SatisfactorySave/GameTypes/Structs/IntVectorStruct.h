#pragma once

#include "../UE/Core/Math/IntVector.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API IntVectorStruct final : public StructImpl<IntVectorStruct, FIntVector> {
    public:
        static constexpr std::string_view TypeName = "IntVector";

        using StructImpl<IntVectorStruct, FIntVector>::StructImpl;
    };
} // namespace SatisfactorySave
