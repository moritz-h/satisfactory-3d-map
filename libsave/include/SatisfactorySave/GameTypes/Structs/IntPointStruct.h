#pragma once

#include "../UE/Core/Math/IntPoint.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API IntPointStruct final : public StructImpl<IntPointStruct, FIntPoint> {
    public:
        static constexpr std::string_view TypeName = "IntPoint";

        using StructImpl<IntPointStruct, FIntPoint>::StructImpl;
    };
} // namespace SatisfactorySave
