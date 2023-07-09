#include "GameTypes/Structs/Vector2DStruct.h"

#include "GameTypes/Structs/Base/StructVisitor.h"

void SatisfactorySave::Vector2DStruct::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::Vector2DStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
