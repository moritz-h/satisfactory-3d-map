#include "RailroadTrackPositionStruct.h"

#include "StructVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::RailroadTrackPositionStruct::RailroadTrackPositionStruct(std::string struct_name,
    std::istream& stream)
    : Struct(std::move(struct_name)) {
    ref_ = ObjectReference(stream);
    offset_ = read<float>(stream);
    forward_ = read<float>(stream);
}

void Satisfactory3DMap::RailroadTrackPositionStruct::serialize(std::ostream& stream) const {
    ref_.serialize(stream);
    write(stream, offset_);
    write(stream, forward_);
}

void Satisfactory3DMap::RailroadTrackPositionStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
