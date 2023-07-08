#include "UInt32Set.h"

#include "SetVisitor.h"

void Satisfactory3DMap::UInt32Set::serialize(Archive& ar) {
    ar << set_;
}

void Satisfactory3DMap::UInt32Set::accept(SetVisitor& v) {
    v.visit(*this);
}
