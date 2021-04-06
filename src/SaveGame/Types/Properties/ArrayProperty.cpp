#include "ArrayProperty.h"

#include <utility>

#include "../Structs/Struct.h"
#include "PropertyVisitor.h"
#include "SaveGame/Types/Guid.h"
#include "SaveGame/Types/ObjectReference.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::ArrayProperty::ArrayProperty(
    std::string property_name, std::string property_type, std::istream& stream)
    : Property(std::move(property_name), std::move(property_type), stream) {
    array_type_ = read_length_string(stream);
    read_assert_zero<int8_t>(stream);

    auto count = read<int32_t>(stream);

    array_ = Array::parse(array_type_, count, stream);
}

void Satisfactory3DMap::ArrayProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
