#include "IntMapTypeList.h"

#include "IO/Archive/IStreamArchive.h"
#include "MapTypeListVisitor.h"

void Satisfactory3DMap::IntMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::IntMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        list_.push_back(dynamic_cast<IStreamArchive&>(ar).read<int32_t>());
    } else {
        ar << list_[i];
    }
}
