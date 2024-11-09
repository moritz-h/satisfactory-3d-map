#pragma once

#include "../UE/Core/Math/Vector2D.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API Vector2DStruct final : public StructImpl<Vector2DStruct, FVector2D> {
    public:
        static constexpr std::string_view TypeName = "Vector2D";

        using StructImpl<Vector2DStruct, FVector2D>::StructImpl;
    };
} // namespace SatisfactorySave
