#include "PropertyCollection.h"

#include "../Utils/StreamUtils.h"
#include "ObjectReference.h"
#include "Properties/ArrayProperty.h"
#include "Properties/BoolProperty.h"
#include "Properties/ByteProperty.h"
#include "Properties/EnumProperty.h"
#include "Properties/FloatProperty.h"
#include "Properties/Int64Property.h"
#include "Properties/IntProperty.h"
#include "Properties/MapProperty.h"
#include "Properties/NameProperty.h"
#include "Properties/ObjectProperty.h"
#include "Properties/StrProperty.h"
#include "Properties/StructProperty.h"
#include "Properties/TextProperty.h"

Satisfactory3DMap::PropertyCollection::PropertyCollection(int32_t length, std::istream& stream) {

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

std::unique_ptr<Satisfactory3DMap::Property> Satisfactory3DMap::PropertyCollection::parseProperty(
    std::istream& stream) {
    std::string property_name = read_length_string(stream);
    if (property_name == "None") {
        return nullptr;
    }

    std::string property_type = read_length_string(stream);

    if (property_type == "ArrayProperty") {
        return std::make_unique<ArrayProperty>(property_name, property_type, stream);
    } else if (property_type == "BoolProperty") {
        return std::make_unique<BoolProperty>(property_name, property_type, stream);
    } else if (property_type == "ByteProperty") {
        return std::make_unique<ByteProperty>(property_name, property_type, stream);
    } else if (property_type == "EnumProperty") {
        return std::make_unique<EnumProperty>(property_name, property_type, stream);
    } else if (property_type == "FloatProperty") {
        return std::make_unique<FloatProperty>(property_name, property_type, stream);
    } else if (property_type == "Int64Property") {
        return std::make_unique<Int64Property>(property_name, property_type, stream);
    } else if (property_type == "IntProperty") {
        return std::make_unique<IntProperty>(property_name, property_type, stream);
    } else if (property_type == "MapProperty") {
        return std::make_unique<MapProperty>(property_name, property_type, stream);
    } else if (property_type == "NameProperty") {
        return std::make_unique<NameProperty>(property_name, property_type, stream);
    } else if (property_type == "ObjectProperty") {
        return std::make_unique<ObjectProperty>(property_name, property_type, stream);
    } else if (property_type == "StrProperty") {
        return std::make_unique<StrProperty>(property_name, property_type, stream);
    } else if (property_type == "StructProperty") {
        return std::make_unique<StructProperty>(property_name, property_type, stream);
    } else if (property_type == "TextProperty") {
        return std::make_unique<TextProperty>(property_name, property_type, stream);
    }

    throw std::runtime_error("Unknown property type: " + property_type);
}
