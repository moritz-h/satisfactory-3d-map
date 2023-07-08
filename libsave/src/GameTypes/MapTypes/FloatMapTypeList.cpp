#include "GameTypes/MapTypes/FloatMapTypeList.h"

#include "GameTypes/MapTypes/MapTypeListVisitor.h"
#include "IO/Archive/IStreamArchive.h"

void SatisfactorySave::FloatMapTypeList::accept(SatisfactorySave::MapTypeListVisitor& v) {
    v.visit(*this);
}

void SatisfactorySave::FloatMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        list_.push_back(dynamic_cast<IStreamArchive&>(ar).read<float>());
    } else {
        ar << list_[i];
    }
}
