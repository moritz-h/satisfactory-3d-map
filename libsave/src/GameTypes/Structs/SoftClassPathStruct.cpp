#include "GameTypes/Structs/SoftClassPathStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

void SatisfactorySave::SoftClassPathStruct::serialize(Archive& ar) {
    ar << AssetPathName_;
    ar << SubPathString_;
}

void SatisfactorySave::SoftClassPathStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
