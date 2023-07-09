#include "GameTypes/Structs/Mods/LBBalancerIndexingStruct.h"

#include "GameTypes/Structs/Base/StructVisitor.h"

void SatisfactorySave::LBBalancerIndexingStruct::serialize(Archive& ar) {
    ar << mNormalIndex;
    ar << mOverflowIndex;
    ar << mFilterIndex;
}

void SatisfactorySave::LBBalancerIndexingStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
