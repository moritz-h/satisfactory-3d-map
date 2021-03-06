#include "RailroadTrackPositionStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::RailroadTrackPositionStruct::serialize(Archive& ar) {
    ar << ref_;
    ar << offset_;
    ar << forward_;
}

void Satisfactory3DMap::RailroadTrackPositionStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
