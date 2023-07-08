#include "TextProperty.h"

#include "IO/Archive/OStreamArchive.h"
#include "PropertyVisitor.h"

void Satisfactory3DMap::TextProperty::serialize(Archive& ar) {
    ar << text_;
}

void Satisfactory3DMap::TextProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
