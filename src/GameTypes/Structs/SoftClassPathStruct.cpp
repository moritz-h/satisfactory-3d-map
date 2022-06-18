#include "SoftClassPathStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::SoftClassPathStruct::serialize(Archive& ar) {
    ar << AssetPathName_;
    ar << SubPathString_;
}

void Satisfactory3DMap::SoftClassPathStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
