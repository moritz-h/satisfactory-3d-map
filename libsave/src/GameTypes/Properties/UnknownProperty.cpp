#include "GameTypes/Properties/UnknownProperty.h"

#include "GameTypes/Properties/Base/PropertyVisitor.h"

void SatisfactorySave::UnknownProperty::serialize(Archive& ar) {
    Value.resize(tag_.Size);
    ar.serializeRaw(Value.data(), tag_.Size);
}

void SatisfactorySave::UnknownProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
