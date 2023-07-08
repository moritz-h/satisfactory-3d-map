#include "GameTypes/Properties/FloatProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::FloatProperty::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::FloatProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
