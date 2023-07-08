#include "GameTypes/MapTypes/NameMapTypeList.h"

#include "GameTypes/MapTypes/MapTypeListVisitor.h"

void SatisfactorySave::NameMapTypeList::accept(SatisfactorySave::MapTypeListVisitor& v) {
    v.visit(*this);
}

void SatisfactorySave::NameMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        FName value;
        ar << value;
        list_.push_back(value);
    } else {
        ar << list_[i];
    }
}
