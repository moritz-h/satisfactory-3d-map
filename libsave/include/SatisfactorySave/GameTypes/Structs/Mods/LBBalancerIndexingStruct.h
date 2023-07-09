#pragma once

#include "../Base/Struct.h"

namespace SatisfactorySave {

    // https://github.com/mklierman/SatisfactoryMods/blob/6a9ed9c315c8f58dbbda22efc788e66566c93321/LoadBalancers/Source/LoadBalancers/Public/LBBuild_ModularLoadBalancer.h#L40-L61
    class LBBalancerIndexingStruct : public Struct {
    public:
        using Struct::Struct;

        void serialize(Archive& ar) override;

        void accept(StructVisitor& v) override;

        [[nodiscard]] int32_t normalIndex() const {
            return mNormalIndex;
        }

        [[nodiscard]] int32_t overflowIndex() const {
            return mOverflowIndex;
        }

        [[nodiscard]] int32_t filterIndex() const {
            return mFilterIndex;
        }

    protected:
        int32_t mNormalIndex = 0;
        int32_t mOverflowIndex = 0;
        int32_t mFilterIndex = 0;
    };
} // namespace SatisfactorySave
