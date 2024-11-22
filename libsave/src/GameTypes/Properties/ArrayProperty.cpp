#include "GameTypes/Properties/ArrayProperty.h"

void SatisfactorySave::ArrayProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        Value = Array::create(ArrayType(), ar);
    } else {
        ar << *Value;
    }
}
