#include "GameTypes/Properties/UInt64Property.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::UInt64Property::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::UInt64Property::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
