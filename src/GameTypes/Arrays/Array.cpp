#include "Array.h"

#include <stdexcept>

#include "ByteArray.h"
#include "EnumArray.h"
#include "IntArray.h"
#include "ObjectArray.h"
#include "StructArray.h"

std::unique_ptr<Satisfactory3DMap::Array> Satisfactory3DMap::Array::create(const std::string& array_type, Archive& ar) {
    std::unique_ptr<Array> array;

    if (array_type == "ByteProperty") {
        array = std::make_unique<ByteArray>(array_type);
    } else if (array_type == "EnumProperty") {
        array = std::make_unique<EnumArray>(array_type);
    } else if (array_type == "InterfaceProperty" || array_type == "ObjectProperty") {
        array = std::make_unique<ObjectArray>(array_type);
    } else if (array_type == "IntProperty") {
        array = std::make_unique<IntArray>(array_type);
    } else if (array_type == "StructProperty") {
        array = std::make_unique<StructArray>(array_type);
    } else {
        throw std::runtime_error("Array type \"" + array_type + "\" not implemented!");
    }

    ar << *array;

    return array;
}
