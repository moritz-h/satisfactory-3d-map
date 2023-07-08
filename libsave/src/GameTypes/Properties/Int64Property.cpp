#include "GameTypes/Properties/Int64Property.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::Int64Property::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::Int64Property::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
