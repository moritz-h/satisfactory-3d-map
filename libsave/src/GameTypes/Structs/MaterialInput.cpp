#include "GameTypes/Structs/MaterialInput.h"

#include "GameTypes/Structs/Base/StructVisitor.h"

void SatisfactorySave::ScalarMaterialInputStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}

void SatisfactorySave::VectorMaterialInputStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
