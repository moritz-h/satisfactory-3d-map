#include "NameMapTypeList.h"

#include "MapTypeListVisitor.h"

void Satisfactory3DMap::NameMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::NameMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        FName value;
        ar << value;
        list_.push_back(value);
    } else {
        ar << list_[i];
    }
}
