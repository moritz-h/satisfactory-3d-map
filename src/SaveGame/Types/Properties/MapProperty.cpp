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

Satisfactory3DMap::MapProperty::MapProperty(std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    key_type_ = read_length_string(stream);
    value_type_ = read_length_string(stream);
    read_assert_zero<int8_t>(stream);

    // TODO unknown
    read_assert_zero<int32_t>(stream);

    auto count = read<int32_t>(stream);

    if (key_type_ == "EnumProperty") {
        keys_ = std::make_unique<EnumMapTypeList>(key_type_);
    } else if (key_type_ == "IntProperty") {
        keys_ = std::make_unique<IntMapTypeList>(key_type_);
    } else if (key_type_ == "ObjectProperty") {
        keys_ = std::make_unique<ObjectMapTypeList>(key_type_);
    } else {
        throw std::runtime_error("Map key type \"" + key_type_ + "\" not implemented!");
    }

    if (value_type_ == "ByteProperty") {
        values_ = std::make_unique<ByteMapTypeList>(value_type_);
    } else if (value_type_ == "IntProperty") {
        values_ = std::make_unique<IntMapTypeList>(value_type_);
    } else if (value_type_ == "StructProperty") {
        values_ = std::make_unique<StructMapTypeList>(value_type_);
    } else {
        throw std::runtime_error("Map value type \"" + value_type_ + "\" not implemented!");
    }

    for (int32_t i = 0; i < count; i++) {
        keys_->parseEntry(stream);
        values_->parseEntry(stream);
    }
}

void Satisfactory3DMap::MapProperty::serialize(std::ostream& stream) const {
    Property::serialize(stream);

    write_length_string(stream, key_type_);
    write_length_string(stream, value_type_);
    write<int8_t>(stream, 0);
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
