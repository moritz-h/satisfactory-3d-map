#include "GameTypes/Properties/BoolProperty.h"

#include "GameTypes/Properties/Base/PropertyVisitor.h"

std::unique_ptr<SatisfactorySave::Property> SatisfactorySave::BoolProperty::clone() const {
    return std::make_unique<BoolProperty>(*this);
}

void SatisfactorySave::BoolProperty::accept(PropertyVisitor& v) {
    v.visit(*this);
}
