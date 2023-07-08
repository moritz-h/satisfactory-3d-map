#include "LBBalancerIndexingStruct.h"

#include "../StructVisitor.h"

void Satisfactory3DMap::LBBalancerIndexingStruct::serialize(Archive& ar) {
    ar << mNormalIndex;
    ar << mOverflowIndex;
    ar << mFilterIndex;
}

void Satisfactory3DMap::LBBalancerIndexingStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
