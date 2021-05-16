#include "Property.h"

#include <utility>

#include "ArrayProperty.h"
#include "BoolProperty.h"
#include "ByteProperty.h"
#include "EnumProperty.h"
#include "FloatProperty.h"
#include "Int64Property.h"
#include "Int8Property.h"
#include "IntProperty.h"
#include "MapProperty.h"
#include "NameProperty.h"
#include "ObjectProperty.h"
#include "StrProperty.h"
#include "StructProperty.h"
#include "TextProperty.h"
#include "Utils/StreamUtils.h"

std::unique_ptr<Satisfactory3DMap::Property> Satisfactory3DMap::Property::parse(std::istream& stream) {
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
    } else if (property_type == "Int8Property") {
        return std::make_unique<Int8Property>(property_name, property_type, stream);
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

Satisfactory3DMap::Property::Property(std::string property_name, std::string property_type, std::istream& stream)
    : property_name_(std::move(property_name)),
      property_type_(std::move(property_type)) {
    size_ = read<int32_t>(stream);
    index_ = read<int32_t>(stream);
}
