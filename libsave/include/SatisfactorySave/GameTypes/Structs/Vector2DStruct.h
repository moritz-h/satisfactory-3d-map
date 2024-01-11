#pragma once

#include "../UE/Math/Vector2D.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API Vector2DStruct : public StructImpl<Vector2DStruct, FVector2D> {
    public:
        using StructImpl<Vector2DStruct, FVector2D>::StructImpl;
    };
} // namespace SatisfactorySave
