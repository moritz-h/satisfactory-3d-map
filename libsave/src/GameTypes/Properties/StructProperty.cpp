#include "GameTypes/Properties/StructProperty.h"

void SatisfactorySave::StructProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        Value = Struct::create(structName(), ar);
    } else {
        ar << *Value;
    }
}
