#include "GameTypes/MapTypes/ObjectMapTypeList.h"

#include "GameTypes/MapTypes/MapTypeListVisitor.h"

void Satisfactory3DMap::ObjectMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::ObjectMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        ObjectReference value;
        ar << value;
        list_.push_back(value);
    } else {
        ar << list_[i];
    }
}
