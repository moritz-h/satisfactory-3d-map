#include "MaterialInput.h"

#include "StructVisitor.h"

void Satisfactory3DMap::ScalarMaterialInputStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::VectorMaterialInputStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
