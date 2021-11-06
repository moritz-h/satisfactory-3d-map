#include "ObjectMapTypeList.h"

#include "MapTypeListVisitor.h"
#include "Utils/StreamUtils.h"

void Satisfactory3DMap::ObjectMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::ObjectMapTypeList::parseEntry(std::istream& stream) {
    list_.emplace_back(ObjectReference(stream));
}

void Satisfactory3DMap::ObjectMapTypeList::serializeEntry(std::ostream& stream, std::size_t i) {
    list_[i].serialize(stream);
}
