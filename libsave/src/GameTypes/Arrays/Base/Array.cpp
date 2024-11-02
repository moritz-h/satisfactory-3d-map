#include "GameTypes/Arrays/Base/Array.h"

#include <stdexcept>

#include "GameTypes/Arrays/Base/ArrayAll.h"

std::shared_ptr<SatisfactorySave::Array> SatisfactorySave::Array::create(const FName& array_type, Archive& ar) {
    std::shared_ptr<Array> array;

    if (array_type == BoolArray::TypeName) {
        array = std::make_shared<BoolArray>();
    } else if (array_type == ByteArray::TypeName) {
        array = std::make_shared<ByteArray>();
    } else if (array_type == EnumArray::TypeName) {
        array = std::make_shared<EnumArray>();
    } else if (array_type == FloatArray::TypeName) {
        array = std::make_shared<FloatArray>();
    } else if (array_type == Int64Array::TypeName) {
        array = std::make_shared<Int64Array>();
    } else if (array_type == Int8Array::TypeName) {
        array = std::make_shared<Int8Array>();
    } else if (array_type == IntArray::TypeName) {
        array = std::make_shared<IntArray>();
    } else if (array_type == InterfaceArray::TypeName) {
        array = std::make_shared<InterfaceArray>();
    } else if (array_type == NameArray::TypeName) {
        array = std::make_shared<NameArray>();
    } else if (array_type == ObjectArray::TypeName) {
        array = std::make_shared<ObjectArray>();
    } else if (array_type == SoftObjectArray::TypeName) {
        array = std::make_shared<SoftObjectArray>();
    } else if (array_type == StrArray::TypeName) {
        array = std::make_shared<StrArray>();
    } else if (array_type == StructArray::TypeName) {
        array = std::make_shared<StructArray>();
    } else {
        throw std::runtime_error("Array type \"" + array_type + "\" not implemented!");
    }

    ar << *array;

    return array;
}
