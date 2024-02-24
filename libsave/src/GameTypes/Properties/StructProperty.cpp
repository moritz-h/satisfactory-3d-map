#include "GameTypes/Properties/StructProperty.h"

#include "GameTypes/Properties/Base/PropertyVisitor.h"

void SatisfactorySave::StructProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        struct_ = Struct::create(structName(), ar);
    } else {
        ar << *struct_;
    }
}

void SatisfactorySave::StructProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
