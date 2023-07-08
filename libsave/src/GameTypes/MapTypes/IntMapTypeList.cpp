#include "GameTypes/MapTypes/IntMapTypeList.h"

#include "GameTypes/MapTypes/MapTypeListVisitor.h"
#include "IO/Archive/IStreamArchive.h"

void SatisfactorySave::IntMapTypeList::accept(SatisfactorySave::MapTypeListVisitor& v) {
    v.visit(*this);
}

void SatisfactorySave::IntMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        list_.push_back(dynamic_cast<IStreamArchive&>(ar).read<int32_t>());
    } else {
        ar << list_[i];
    }
}
