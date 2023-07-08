#include "GameTypes/Properties/ObjectProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::ObjectProperty::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::ObjectProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
