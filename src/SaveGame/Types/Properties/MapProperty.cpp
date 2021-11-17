#include "MapProperty.h"

#include <utility>

#include "../MapTypes/ByteMapTypeList.h"
#include "../MapTypes/EnumMapTypeList.h"
#include "../MapTypes/IntMapTypeList.h"
#include "../MapTypes/ObjectMapTypeList.h"
#include "../MapTypes/StructMapTypeList.h"
#include "../ObjectReference.h"
#include "../Structs/PropertyStruct.h"
#include "PropertyVisitor.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::MapProperty::MapProperty(PropertyTag tag, std::istream& stream) : Property(std::move(tag)) {

    // TODO unknown
    read_assert_zero<int32_t>(stream);

    auto count = read<int32_t>(stream);

    if (tag_.InnerType == "EnumProperty") {
        keys_ = std::make_unique<EnumMapTypeList>(tag_.InnerType);
    } else if (tag_.InnerType == "IntProperty") {
        keys_ = std::make_unique<IntMapTypeList>(tag_.InnerType);
    } else if (tag_.InnerType == "ObjectProperty") {
        keys_ = std::make_unique<ObjectMapTypeList>(tag_.InnerType);
    } else {
        throw std::runtime_error("Map key type \"" + tag_.InnerType + "\" not implemented!");
    }

    if (tag_.ValueType == "ByteProperty") {
        values_ = std::make_unique<ByteMapTypeList>(tag_.ValueType);
    } else if (tag_.ValueType == "IntProperty") {
        values_ = std::make_unique<IntMapTypeList>(tag_.ValueType);
    } else if (tag_.ValueType == "StructProperty") {
        values_ = std::make_unique<StructMapTypeList>(tag_.ValueType);
    } else {
        throw std::runtime_error("Map value type \"" + tag_.ValueType + "\" not implemented!");
    }

    for (int32_t i = 0; i < count; i++) {
        keys_->parseEntry(stream);
        values_->parseEntry(stream);
    }
}

void Satisfactory3DMap::MapProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);

    write<int32_t>(stream, 0);

    auto count = keys_->listSize();
    if (count != values_->listSize()) {
        throw std::runtime_error("Invalid map size!");
    }
    write(stream, static_cast<int32_t>(count));

    for (std::size_t i = 0; i < count; i++) {
        keys_->serializeEntry(stream, i);
        values_->serializeEntry(stream, i);
    }
}

void Satisfactory3DMap::MapProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
