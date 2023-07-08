#include "GameTypes/Properties/UnknownProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::UnknownProperty::serialize(Archive& ar) {
    value_.resize(tag_.Size);
    ar.serializeRaw(value_.data(), tag_.Size);
}

void SatisfactorySave::UnknownProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
