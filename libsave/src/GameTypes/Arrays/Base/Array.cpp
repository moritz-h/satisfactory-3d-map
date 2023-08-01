#include "GameTypes/Arrays/Base/Array.h"

#include <stdexcept>

#include "GameTypes/Arrays/Base/ArrayAll.h"

std::unique_ptr<SatisfactorySave::Array> SatisfactorySave::Array::create(const FName& array_type, Archive& ar) {
    std::unique_ptr<Array> array;

    if (array_type == BoolArray::TypeName) {
        array = std::make_unique<BoolArray>();
    } else if (array_type == ByteArray::TypeName) {
        array = std::make_unique<ByteArray>();
    } else if (array_type == EnumArray::TypeName) {
        array = std::make_unique<EnumArray>();
    } else if (array_type == FloatArray::TypeName) {
        array = std::make_unique<FloatArray>();
    } else if (array_type == Int64Array::TypeName) {
        array = std::make_unique<Int64Array>();
    } else if (array_type == IntArray::TypeName) {
        array = std::make_unique<IntArray>();
    } else if (array_type == InterfaceArray::TypeName) {
        array = std::make_unique<InterfaceArray>();
    } else if (array_type == NameArray::TypeName) {
        array = std::make_unique<NameArray>();
    } else if (array_type == ObjectArray::TypeName) {
        array = std::make_unique<ObjectArray>();
    } else if (array_type == SoftObjectArray::TypeName) {
        array = std::make_unique<SoftObjectArray>();
    } else if (array_type == StrArray::TypeName) {
        array = std::make_unique<StrArray>();
    } else if (array_type == StructArray::TypeName) {
        array = std::make_unique<StructArray>();
    } else {
        throw std::runtime_error("Array type \"" + array_type + "\" not implemented!");
    }

    ar << *array;

    return array;
}
