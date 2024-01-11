#pragma once

#include "../UE/Math/Color.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ColorStruct : public StructImpl<ColorStruct, FColor> {
    public:
        using StructImpl<ColorStruct, FColor>::StructImpl;
    };
} // namespace SatisfactorySave
