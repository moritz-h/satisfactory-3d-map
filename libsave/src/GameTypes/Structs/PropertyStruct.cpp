#include "GameTypes/Structs/PropertyStruct.h"

#include "GameTypes/Structs/StructVisitor.h"

SatisfactorySave::PropertyStruct::PropertyStruct(SatisfactorySave::FName name) : Struct(std::move(name)) {
    properties_.setParentClass(name_.toString());
}

void SatisfactorySave::PropertyStruct::serialize(Archive& ar) {
    ar << properties_;
}

void SatisfactorySave::PropertyStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
