#include "ByteMapTypeList.h"

#include "MapTypeListVisitor.h"
#include "Utils/StreamUtils.h"

void Satisfactory3DMap::ByteMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::ByteMapTypeList::parseEntry(std::istream& stream) {
    list_.emplace_back(read<int8_t>(stream));
}

void Satisfactory3DMap::ByteMapTypeList::serializeEntry(std::ostream& stream, std::size_t i) {
    write(stream, list_[i]);
}
