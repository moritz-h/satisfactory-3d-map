#include "Array.h"

#include <stdexcept>

#include "BoolArray.h"
#include "ByteArray.h"
#include "EnumArray.h"
#include "FloatArray.h"
#include "Int64Array.h"
#include "IntArray.h"
#include "NameArray.h"
#include "ObjectArray.h"
#include "SoftObjectArray.h"
#include "StrArray.h"
#include "StructArray.h"

std::unique_ptr<Satisfactory3DMap::Array> Satisfactory3DMap::Array::create(const FName& array_type, Archive& ar) {
    std::unique_ptr<Array> array;

    if (array_type == "BoolProperty") {
        array = std::make_unique<BoolArray>(array_type);
    } else if (array_type == "ByteProperty") {
        array = std::make_unique<ByteArray>(array_type);
    } else if (array_type == "EnumProperty") {
        array = std::make_unique<EnumArray>(array_type);
    } else if (array_type == "FloatProperty") {
        array = std::make_unique<FloatArray>(array_type);
    } else if (array_type == "InterfaceProperty" || array_type == "ObjectProperty") {
        array = std::make_unique<ObjectArray>(array_type);
    } else if (array_type == "Int64Property") {
        array = std::make_unique<Int64Array>(array_type);
    } else if (array_type == "IntProperty") {
        array = std::make_unique<IntArray>(array_type);
    } else if (array_type == "NameProperty") {
        array = std::make_unique<NameArray>(array_type);
    } else if (array_type == "SoftObjectProperty") {
        array = std::make_unique<SoftObjectArray>(array_type);
    } else if (array_type == "StrProperty") {
        array = std::make_unique<StrArray>(array_type);
    } else if (array_type == "StructProperty") {
        array = std::make_unique<StructArray>(array_type);
    } else {
        throw std::runtime_error("Array type \"" + array_type + "\" not implemented!");
    }

    ar << *array;

    return array;
}
