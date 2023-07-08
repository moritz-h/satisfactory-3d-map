#include "GameTypes/Properties/NameProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::NameProperty::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::NameProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
