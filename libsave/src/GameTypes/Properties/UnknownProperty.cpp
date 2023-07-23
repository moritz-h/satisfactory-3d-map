#include "GameTypes/Properties/UnknownProperty.h"

#include "GameTypes/Properties/Base/PropertyVisitor.h"

void SatisfactorySave::UnknownProperty::serialize(Archive& ar) {
    value_.resize(Tag.Size);
    ar.serializeRaw(value_.data(), Tag.Size);
}

void SatisfactorySave::UnknownProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
