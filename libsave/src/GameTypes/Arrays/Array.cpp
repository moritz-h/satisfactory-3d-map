#include "GameTypes/Arrays/Array.h"

#include <stdexcept>

#include "GameTypes/Arrays/BoolArray.h"
#include "GameTypes/Arrays/ByteArray.h"
#include "GameTypes/Arrays/EnumArray.h"
#include "GameTypes/Arrays/FloatArray.h"
#include "GameTypes/Arrays/Int64Array.h"
#include "GameTypes/Arrays/IntArray.h"
#include "GameTypes/Arrays/NameArray.h"
#include "GameTypes/Arrays/ObjectArray.h"
#include "GameTypes/Arrays/SoftObjectArray.h"
#include "GameTypes/Arrays/StrArray.h"
#include "GameTypes/Arrays/StructArray.h"

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
