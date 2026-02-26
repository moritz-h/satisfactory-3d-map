#include "GameTypes/Properties/ArrayProperty.h"

void SatisfactorySave::ArrayProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        const auto type_param = !tag_.TypeName.IsEmpty() ? tag_.TypeName.GetParameter(0) : FPropertyTypeName();
        Value = Array::create(ArrayType(), ar, type_param);
    } else {
        ar << *Value;
    }
}
