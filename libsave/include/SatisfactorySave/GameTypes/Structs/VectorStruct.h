#pragma once

#include "../UE/Core/Math/Vector.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API VectorStruct final : public StructImpl<VectorStruct, FVector> {
    public:
        static constexpr std::string_view TypeName = "Vector";

        using StructImpl<VectorStruct, FVector>::StructImpl;
    };
} // namespace SatisfactorySave
