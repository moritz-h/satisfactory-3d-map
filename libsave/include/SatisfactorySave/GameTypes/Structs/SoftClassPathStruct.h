#pragma once

#include "../UE/UObject/SoftObjectPath.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SoftClassPathStruct : public StructImpl<SoftClassPathStruct, FSoftObjectPath> {
    public:
        using StructImpl<SoftClassPathStruct, FSoftObjectPath>::StructImpl;
    };
} // namespace SatisfactorySave
