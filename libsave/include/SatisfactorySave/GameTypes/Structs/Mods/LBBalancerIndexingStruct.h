#pragma once

#include "../../UE/Satisfactory/Mods/LBBalancerIndexing.h"
#include "../Base/StructImpl.h"

namespace SatisfactorySave {

    class LBBalancerIndexingStruct : public StructImpl<LBBalancerIndexingStruct, FLBBalancerIndexing> {
    public:
        using StructImpl<LBBalancerIndexingStruct, FLBBalancerIndexing>::StructImpl;
    };
} // namespace SatisfactorySave
