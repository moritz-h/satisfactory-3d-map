#include "ByteMapTypeList.h"

#include "IO/Archive/IStreamArchive.h"
#include "MapTypeListVisitor.h"

void Satisfactory3DMap::ByteMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::ByteMapTypeList::serializeEntry(Archive& ar, std::size_t i) {
    if (ar.isIArchive()) {
        list_.push_back(dynamic_cast<IStreamArchive&>(ar).read<int8_t>());
    } else {
        ar << list_[i];
    }
}
