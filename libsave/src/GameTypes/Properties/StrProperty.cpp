#include "GameTypes/Properties/StrProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::StrProperty::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::StrProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
