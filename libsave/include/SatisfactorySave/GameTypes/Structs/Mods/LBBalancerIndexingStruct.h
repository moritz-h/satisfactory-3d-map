#pragma once

#include "../../Mods/LBBalancerIndexing.h"
#include "../Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API LBBalancerIndexingStruct final
        : public StructImpl<LBBalancerIndexingStruct, FLBBalancerIndexing> {
    public:
        static constexpr std::string_view TypeName = "LBBalancerIndexing";

        using StructImpl<LBBalancerIndexingStruct, FLBBalancerIndexing>::StructImpl;
    };
} // namespace SatisfactorySave
