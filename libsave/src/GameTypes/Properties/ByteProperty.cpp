#include "GameTypes/Properties/ByteProperty.h"

void SatisfactorySave::ByteProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        if (EnumName() == "None") {
            Value = static_cast<int8_t>(0);
        } else {
            Value = FName();
        }
    }
    if (EnumName() == "None") {
        ar << std::get<int8_t>(Value);
    } else {
        ar << std::get<FName>(Value);
    }
}
