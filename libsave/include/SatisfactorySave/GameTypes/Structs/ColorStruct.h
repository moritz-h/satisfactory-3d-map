#pragma once

#include "../UE/Math/Color.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class ColorStruct : public StructImpl<ColorStruct, FColor> {
    public:
        using StructImpl<ColorStruct, FColor>::StructImpl;
    };
} // namespace SatisfactorySave
