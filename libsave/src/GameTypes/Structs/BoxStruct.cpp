#include "GameTypes/Structs/BoxStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void SatisfactorySave::BoxStruct::serialize(Archive& ar) {
    ar << min_;
    ar << max_;
    ar << is_valid_;
}

void SatisfactorySave::BoxStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
