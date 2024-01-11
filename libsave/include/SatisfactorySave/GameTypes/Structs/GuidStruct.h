#pragma once

#include "../UE/Misc/Guid.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API GuidStruct : public StructImpl<GuidStruct, FGuid> {
    public:
        using StructImpl<GuidStruct, FGuid>::StructImpl;
    };
} // namespace SatisfactorySave
