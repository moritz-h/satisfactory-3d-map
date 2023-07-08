#include "GameTypes/Properties/TextProperty.h"

#include "GameTypes/Properties/PropertyVisitor.h"
#include "IO/Archive/OStreamArchive.h"

void SatisfactorySave::TextProperty::serialize(Archive& ar) {
    ar << text_;
}

void SatisfactorySave::TextProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
