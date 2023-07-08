#include "GameTypes/Properties/DoubleProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::DoubleProperty::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::DoubleProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
