#include "IntMapTypeList.h"

#include "MapTypeListVisitor.h"
#include "Utils/StreamUtils.h"

void Satisfactory3DMap::IntMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::IntMapTypeList::parseEntry(std::istream& stream) {
    list_.emplace_back(read<int32_t>(stream));
}
