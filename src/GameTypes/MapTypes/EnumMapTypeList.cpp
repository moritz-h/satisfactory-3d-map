#include "EnumMapTypeList.h"

#include "MapTypeListVisitor.h"
#include "Utils/StreamUtils.h"

void Satisfactory3DMap::EnumMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::EnumMapTypeList::parseEntry(std::istream& stream) {
    list_.emplace_back(read_length_string(stream));
}

void Satisfactory3DMap::EnumMapTypeList::serializeEntry(std::ostream& stream, std::size_t i) {
    write_length_string(stream, list_[i]);
}
