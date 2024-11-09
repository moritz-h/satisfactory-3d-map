#pragma once

#include "../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // FLBBalancerIndexing
    // https://github.com/mklierman/SatisfactoryMods/blob/6a9ed9c315c8f58dbbda22efc788e66566c93321/LoadBalancers/Source/LoadBalancers/Public/LBBuild_ModularLoadBalancer.h#L40-L61
    struct SATISFACTORYSAVE_API FLBBalancerIndexing {
    public:
        int32_t mNormalIndex;
        int32_t mOverflowIndex;
        int32_t mFilterIndex;

        void serialize(Archive& ar) {
            ar << mNormalIndex;
            ar << mOverflowIndex;
            ar << mFilterIndex;
        }
    };
} // namespace SatisfactorySave
