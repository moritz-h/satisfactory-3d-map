#include "GameTypes/Properties/IntProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::IntProperty::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::IntProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
