#pragma once

#include "../UE/Math/Box.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class BoxStruct : public StructImpl<BoxStruct, FBox> {
    public:
        using StructImpl<BoxStruct, FBox>::StructImpl;
    };
} // namespace SatisfactorySave
