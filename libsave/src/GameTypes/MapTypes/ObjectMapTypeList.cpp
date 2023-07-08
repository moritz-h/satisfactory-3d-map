#include "GameTypes/MapTypes/ObjectMapTypeList.h"

#include "GameTypes/MapTypes/MapTypeListVisitor.h"

void SatisfactorySave::ObjectMapTypeList::accept(SatisfactorySave::MapTypeListVisitor& v) {
    v.visit(*this);
}

void SatisfactorySave::ObjectMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        ObjectReference value;
        ar << value;
        list_.push_back(value);
    } else {
        ar << list_[i];
    }
}
