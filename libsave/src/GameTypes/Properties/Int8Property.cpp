#include "GameTypes/Properties/Int8Property.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::Int8Property::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::Int8Property::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
