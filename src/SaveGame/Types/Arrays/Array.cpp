#include "Array.h"

#include <stdexcept>

#include "ByteArray.h"
#include "EnumArray.h"
#include "IntArray.h"
#include "ObjectArray.h"
#include "StructArray.h"

std::unique_ptr<Satisfactory3DMap::Array> Satisfactory3DMap::Array::parse(const std::string& array_type, int32_t count,
    std::istream& stream) {
    if (array_type == "ByteProperty") {
        return std::make_unique<ByteArray>(array_type, count, stream);
    } else if (array_type == "EnumProperty") {
        return std::make_unique<EnumArray>(array_type, count, stream);
    } else if (array_type == "InterfaceProperty" || array_type == "ObjectProperty") {
        return std::make_unique<ObjectArray>(array_type, count, stream);
    } else if (array_type == "IntProperty") {
        return std::make_unique<IntArray>(array_type, count, stream);
    } else if (array_type == "StructProperty") {
        return std::make_unique<StructArray>(array_type, count, stream);
    } else {
        throw std::runtime_error("Array type \"" + array_type + "\" not implemented!");
    }
}
