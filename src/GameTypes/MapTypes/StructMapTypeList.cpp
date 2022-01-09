#include "StructMapTypeList.h"

#include "MapTypeListVisitor.h"

void Satisfactory3DMap::StructMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::StructMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        auto propertyStruct = std::make_unique<PropertyStruct>("");
        ar << *propertyStruct;
        list_.push_back(std::move(propertyStruct));
    } else {
        ar << *list_[i];
    }
}
