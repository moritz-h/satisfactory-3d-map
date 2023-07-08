#include "GameTypes/MapTypes/EnumMapTypeList.h"

#include "GameTypes/MapTypes/MapTypeListVisitor.h"

void Satisfactory3DMap::EnumMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::EnumMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        std::string value;
        ar << value;
        list_.push_back(value);
    } else {
        ar << list_[i];
    }
}
