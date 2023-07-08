#include "GameTypes/Properties/TextProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"
#include "IO/Archive/OStreamArchive.h"

void Satisfactory3DMap::TextProperty::serialize(Archive& ar) {
    ar << text_;
}

void Satisfactory3DMap::TextProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
