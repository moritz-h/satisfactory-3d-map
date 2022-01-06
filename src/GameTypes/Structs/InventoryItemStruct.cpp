#include "InventoryItemStruct.h"

#include "StructVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::InventoryItemStruct::InventoryItemStruct(std::string struct_name, std::istream& stream)
    : Struct(std::move(struct_name)) {
    unk1_ = read<int32_t>(stream);
    class_name_ = read_length_string(stream);
    ref_ = ObjectReference(stream);
}

void Satisfactory3DMap::InventoryItemStruct::serialize(std::ostream& stream) const {
    write(stream, unk1_);
    write_length_string(stream, class_name_);
    ref_.serialize(stream);
}

void Satisfactory3DMap::InventoryItemStruct::accept(Satisfactory3DMap::StructVisitor& v) {
    v.visit(*this);
}
