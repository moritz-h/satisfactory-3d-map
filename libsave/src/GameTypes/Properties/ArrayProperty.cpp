#include "GameTypes/Properties/ArrayProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::ArrayProperty::serialize(SatisfactorySave::Archive& ar) {
    if (ar.isIArchive()) {
        array_ = Array::create(tag_.InnerType, ar);
    } else {
        ar << *array_;
    }
}

void SatisfactorySave::ArrayProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
