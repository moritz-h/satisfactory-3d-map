#include "GameTypes/Properties/ArrayProperty.h"

void SatisfactorySave::ArrayProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        Value = Array::create(arrayType(), ar);
    } else {
        ar << *Value;
    }
}
