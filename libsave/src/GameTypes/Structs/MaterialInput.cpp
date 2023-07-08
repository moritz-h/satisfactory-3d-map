#include "GameTypes/Structs/MaterialInput.h"

#include "GameTypes/Structs/StructVisitor.h"

void Satisfactory3DMap::ScalarMaterialInputStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::VectorMaterialInputStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
