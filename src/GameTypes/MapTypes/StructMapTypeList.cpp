#include "StructMapTypeList.h"

#include "MapTypeListVisitor.h"

Satisfactory3DMap::StructMapTypeList::StructMapTypeList(FName type, const FName& name,
    const std::string& parentClassName)
    : MapTypeList(std::move(type)) {
    struct_name_.Name = "TODO"; // TODO
}

void Satisfactory3DMap::StructMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::StructMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        auto s = Struct::create(struct_name_, ar);
        list_.push_back(std::move(s));
    } else {
        ar << *list_[i];
    }
}
