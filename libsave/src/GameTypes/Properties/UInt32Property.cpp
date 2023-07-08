#include "GameTypes/Properties/UInt32Property.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::UInt32Property::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::UInt32Property::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
