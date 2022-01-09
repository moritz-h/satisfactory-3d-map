#include "StructProperty.h"

#include "PropertyVisitor.h"

void Satisfactory3DMap::StructProperty::serialize(Archive& ar) {
    Property::serialize(ar);

    if (ar.isIArchive()) {
        struct_ = Struct::create(tag_.StructName, ar);
    } else {
        ar << *struct_;
    }
}

void Satisfactory3DMap::StructProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
