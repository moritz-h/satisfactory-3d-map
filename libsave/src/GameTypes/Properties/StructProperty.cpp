#include "GameTypes/Properties/StructProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"

void SatisfactorySave::StructProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        struct_ = Struct::create(tag_.StructName, ar);
    } else {
        ar << *struct_;
    }
}

void SatisfactorySave::StructProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
