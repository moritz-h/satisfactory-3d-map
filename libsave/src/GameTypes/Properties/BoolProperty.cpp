#include "GameTypes/Properties/BoolProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::BoolProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
