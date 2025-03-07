#include "GameTypes/Properties/UnknownProperty.h"

#include "GameTypes/Properties/Base/PropertyVisitor.h"

std::shared_ptr<SatisfactorySave::Property> SatisfactorySave::UnknownProperty::clone() const {
    return std::make_shared<UnknownProperty>(*this);
}

void SatisfactorySave::UnknownProperty::serialize(Archive& ar) {
    Value.resize(tag_.Size);
    ar.serializeRaw(Value.data(), tag_.Size);
}

void SatisfactorySave::UnknownProperty::accept(PropertyVisitor& v) {
    v.visit(*this);
}
