#include "GameTypes/Structs/RailroadTrackPositionStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void SatisfactorySave::RailroadTrackPositionStruct::serialize(Archive& ar) {
    ar << ref_;
    ar << offset_;
    ar << forward_;
}

void SatisfactorySave::RailroadTrackPositionStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
