#include "InventoryItemStruct.h"

#include "StructVisitor.h"

void Satisfactory3DMap::InventoryItemStruct::serialize(Archive& ar) {
    ar << unk1_;
    ar << class_name_;
    ar << ref_;
}

void Satisfactory3DMap::InventoryItemStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
