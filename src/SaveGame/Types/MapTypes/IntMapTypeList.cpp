#include "IntMapTypeList.h"

#include "MapTypeListVisitor.h"
#include "Utils/StreamUtils.h"

void Satisfactory3DMap::IntMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::IntMapTypeList::parseEntry(std::istream& stream) {
    list_.emplace_back(read<int32_t>(stream));
}

void Satisfactory3DMap::IntMapTypeList::serializeEntry(std::ostream& stream, std::size_t i) {
    write(stream, list_[i]);
}
