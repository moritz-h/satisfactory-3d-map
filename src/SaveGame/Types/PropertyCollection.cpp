#include "PropertyCollection.h"

#include "../Utils/FileUtils.h"
#include "ObjectReference.h"

SatisfactorySaveGame::PropertyCollection::PropertyCollection(int32_t length, std::istream& stream) {

    auto pos_before = stream.tellg();

    bool done = false;
    do {
        auto property = parseProperty(stream);
        if (property == nullptr) {
            done = true;
        }
    } while (!done);

    read<int32_t>(stream);

    auto pos_after = stream.tellg();

    // TODO
    if (pos_after - pos_before != length) {
        stream.ignore(length - (pos_after - pos_before));
    }
}

std::unique_ptr<SatisfactorySaveGame::Property> SatisfactorySaveGame::PropertyCollection::parseProperty(
    std::istream& stream) {
    std::string property_name = read_length_string(stream);
    if (property_name == "None") {
        return nullptr;
    }

    std::string property_type = read_length_string(stream);
    auto size = read<int32_t>(stream);
    auto index = read<int32_t>(stream);

    if (property_type == "ArrayProperty") {
        auto array_type = read_length_string(stream);
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("ArrayProperty: Unexpected none zero byte!");
        }
        stream.ignore(size); // TODO
    } else if (property_type == "BoolProperty") {
        auto value = read<int8_t>(stream);
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("BoolProperty: Unexpected none zero byte!");
        }
    } else if (property_type == "ByteProperty") {
        auto byte_type = read_length_string(stream);
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("ByteProperty: Unexpected none zero byte!");
        }
        stream.ignore(size); // TODO
    } else if (property_type == "EnumProperty") {
        auto enum_type = read_length_string(stream);
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("EnumProperty: Unexpected none zero byte!");
        }
        auto value = read_length_string(stream);
    } else if (property_type == "FloatProperty") {
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("FloatProperty: Unexpected none zero byte!");
        }
        auto value = read<float>(stream);
    } else if (property_type == "IntProperty") {
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("IntProperty: Unexpected none zero byte!");
        }
        auto value = read<int32_t>(stream);
    } else if (property_type == "Int64Property") {
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("Int64Property: Unexpected none zero byte!");
        }
        auto value = read<int64_t>(stream);
    } else if (property_type == "MapProperty") {
        auto key_type = read_length_string(stream);
        auto value_type = read_length_string(stream);
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("MapProperty: Unexpected none zero byte!");
        }
        stream.ignore(size); // TODO
    } else if (property_type == "NameProperty") {
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("NameProperty: Unexpected none zero byte!");
        }
        auto value = read_length_string(stream);
    } else if (property_type == "ObjectProperty") {
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("ObjectProperty: Unexpected none zero byte!");
        }
        ObjectReference ref;
        ref.level_name = read_length_string(stream);
        ref.path_name = read_length_string(stream);
    } else if (property_type == "StrProperty") {
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("StrProperty: Unexpected none zero byte!");
        }
        auto value = read_length_string(stream);
    } else if (property_type == "StructProperty") {
        auto struct_name = read_length_string(stream);
        read<int32_t>(stream);
        read<int32_t>(stream);
        read<int32_t>(stream);
        read<int32_t>(stream);
        read<int8_t>(stream);
        stream.ignore(size); // TODO
    } else if (property_type == "TextProperty") {
        auto unk = read<int8_t>(stream);
        if (unk != 0) {
            throw std::runtime_error("TextProperty: Unexpected none zero byte!");
        }
        stream.ignore(size); // TODO
    } else {
        throw std::runtime_error("Unknown property type: " + property_type);
    }

    return std::make_unique<Property>();
}
