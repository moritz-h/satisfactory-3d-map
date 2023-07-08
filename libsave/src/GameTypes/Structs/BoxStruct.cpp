#include "GameTypes/Structs/BoxStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void Satisfactory3DMap::BoxStruct::serialize(Archive& ar) {
    ar << min_;
    ar << max_;
    ar << is_valid_;
}

void Satisfactory3DMap::BoxStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
