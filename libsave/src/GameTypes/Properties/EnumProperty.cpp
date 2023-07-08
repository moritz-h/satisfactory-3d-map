#include "GameTypes/Properties/EnumProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::EnumProperty::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::EnumProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
