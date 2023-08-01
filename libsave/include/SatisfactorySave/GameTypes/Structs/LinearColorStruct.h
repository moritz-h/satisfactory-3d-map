#pragma once

#include "../UE/Math/Color.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class LinearColorStruct : public StructImpl<LinearColorStruct, FLinearColor> {
    public:
        using StructImpl<LinearColorStruct, FLinearColor>::StructImpl;
    };
} // namespace SatisfactorySave
