#include "PropertyStruct.h"

#include "StructVisitor.h"

Satisfactory3DMap::PropertyStruct::PropertyStruct(Satisfactory3DMap::FName name) : Struct(std::move(name)) {
    properties_.setParentClass(name_.toString());
}

void Satisfactory3DMap::PropertyStruct::serialize(Archive& ar) {
    ar << properties_;
}

void Satisfactory3DMap::PropertyStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
