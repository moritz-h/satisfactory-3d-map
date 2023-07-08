#include "GameTypes/MapTypes/ByteMapTypeList.h"

#include "GameTypes/MapTypes/MapTypeListVisitor.h"
#include "IO/Archive/IStreamArchive.h"

void SatisfactorySave::ByteMapTypeList::accept(SatisfactorySave::MapTypeListVisitor& v) {
    v.visit(*this);
}

void SatisfactorySave::ByteMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        list_.push_back(dynamic_cast<IStreamArchive&>(ar).read<int8_t>());
    } else {
        ar << list_[i];
    }
}
