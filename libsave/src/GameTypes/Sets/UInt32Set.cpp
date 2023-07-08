#include "GameTypes/Sets/UInt32Set.h"

#include "GameTypes/Sets/SetVisitor.h"

void SatisfactorySave::UInt32Set::serialize(Archive& ar) {
    ar << set_;
}

void SatisfactorySave::UInt32Set::accept(SetVisitor& v) {
    v.visit(*this);
}
