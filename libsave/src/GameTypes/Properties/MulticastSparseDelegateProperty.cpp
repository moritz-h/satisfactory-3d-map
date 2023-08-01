#include "GameTypes/Properties/MulticastSparseDelegateProperty.h"

#include "GameTypes/Properties/Base/PropertyVisitor.h"

void SatisfactorySave::MulticastSparseDelegateProperty::serialize(Archive& ar) {
    ar << InvocationList_;
}

void SatisfactorySave::MulticastSparseDelegateProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
