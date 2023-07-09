#include "GameTypes/Structs/InventoryItemStruct.h"

#include "GameTypes/Structs/Base/StructVisitor.h"

void SatisfactorySave::InventoryItemStruct::serialize(Archive& ar) {
    ar << unk1_;
    ar << class_name_;
    ar << ref_;
}

void SatisfactorySave::InventoryItemStruct::accept(SatisfactorySave::StructVisitor& v) {
    v.visit(*this);
}
