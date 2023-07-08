#include "GameTypes/MapTypes/FloatMapTypeList.h"

#include "GameTypes/MapTypes/MapTypeListVisitor.h"
#include "IO/Archive/IStreamArchive.h"

void Satisfactory3DMap::FloatMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::FloatMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        list_.push_back(dynamic_cast<IStreamArchive&>(ar).read<float>());
    } else {
        ar << list_[i];
    }
}
