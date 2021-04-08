#include "StructMapTypeList.h"

#include "MapTypeListVisitor.h"
#include "Utils/StreamUtils.h"

void Satisfactory3DMap::StructMapTypeList::accept(Satisfactory3DMap::MapTypeListVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::StructMapTypeList::parseEntry(std::istream& stream) {
    list_.emplace_back(std::make_unique<PropertyStruct>("", stream));
}
