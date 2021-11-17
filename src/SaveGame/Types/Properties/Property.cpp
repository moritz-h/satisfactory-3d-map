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
#include "PropertyTag.h"
#include "StrProperty.h"
#include "StructProperty.h"
#include "TextProperty.h"
#include "Utils/StreamUtils.h"

std::unique_ptr<Satisfactory3DMap::Property> Satisfactory3DMap::Property::parse(std::istream& stream) {
    PropertyTag tag(stream);

    if (tag.Name == "None") {
        return nullptr;
    }

    if (tag.Type == "ArrayProperty") {
        return std::make_unique<ArrayProperty>(std::move(tag), stream);
    } else if (tag.Type == "BoolProperty") {
        return std::make_unique<BoolProperty>(std::move(tag));
    } else if (tag.Type == "ByteProperty") {
        return std::make_unique<ByteProperty>(std::move(tag), stream);
    } else if (tag.Type == "EnumProperty") {
        return std::make_unique<EnumProperty>(std::move(tag), stream);
    } else if (tag.Type == "FloatProperty") {
        return std::make_unique<FloatProperty>(std::move(tag), stream);
    } else if (tag.Type == "Int64Property") {
        return std::make_unique<Int64Property>(std::move(tag), stream);
    } else if (tag.Type == "Int8Property") {
        return std::make_unique<Int8Property>(std::move(tag), stream);
    } else if (tag.Type == "IntProperty") {
        return std::make_unique<IntProperty>(std::move(tag), stream);
    } else if (tag.Type == "MapProperty") {
        return std::make_unique<MapProperty>(std::move(tag), stream);
    } else if (tag.Type == "NameProperty") {
        return std::make_unique<NameProperty>(std::move(tag), stream);
    } else if (tag.Type == "ObjectProperty") {
        return std::make_unique<ObjectProperty>(std::move(tag), stream);
    } else if (tag.Type == "StrProperty") {
        return std::make_unique<StrProperty>(std::move(tag), stream);
    } else if (tag.Type == "StructProperty") {
        return std::make_unique<StructProperty>(std::move(tag), stream);
    } else if (tag.Type == "TextProperty") {
        return std::make_unique<TextProperty>(std::move(tag), stream);
    }

    throw std::runtime_error("Unknown property type: " + tag.Type);
}

Satisfactory3DMap::Property::Property(Satisfactory3DMap::PropertyTag tag) : tag_(std::move(tag)) {}

void Satisfactory3DMap::Property::serialize(std::ostream& stream) const {
    tag_.serialize(stream);
    // TODO serialize size from children
}
