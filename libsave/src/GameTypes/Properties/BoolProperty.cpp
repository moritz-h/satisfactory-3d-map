#include "GameTypes/Properties/BoolProperty.h"

#include "GameTypes/Properties/Base/PropertyVisitor.h"

std::unique_ptr<SatisfactorySave::Property> SatisfactorySave::BoolProperty::clone() {
    return std::make_unique<BoolProperty>(*this);
}

void SatisfactorySave::BoolProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
