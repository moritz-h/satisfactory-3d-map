#include "GameTypes/Structs/Mods/LBBalancerIndexingStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void Satisfactory3DMap::LBBalancerIndexingStruct::serialize(Archive& ar) {
    ar << mNormalIndex;
    ar << mOverflowIndex;
    ar << mFilterIndex;
}

void Satisfactory3DMap::LBBalancerIndexingStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
