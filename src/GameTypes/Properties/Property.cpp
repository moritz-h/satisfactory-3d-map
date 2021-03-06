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
#include "MulticastSparseDelegateProperty.h"
#include "NameProperty.h"
#include "ObjectProperty.h"
#include "PropertyTag.h"
#include "SetProperty.h"
#include "StrProperty.h"
#include "StructProperty.h"
#include "TextProperty.h"

std::unique_ptr<Satisfactory3DMap::Property> Satisfactory3DMap::Property::create(Satisfactory3DMap::IStreamArchive& ar,
    const std::string& parentClassName) {
    PropertyTag tag;
    ar << tag;

    if (tag.Name == "None") {
        return nullptr;
    }

    std::unique_ptr<Property> property;

    if (tag.Type == "ArrayProperty") {
        property = std::make_unique<ArrayProperty>(std::move(tag));
    } else if (tag.Type == "BoolProperty") {
        property = std::make_unique<BoolProperty>(std::move(tag));
    } else if (tag.Type == "ByteProperty") {
        property = std::make_unique<ByteProperty>(std::move(tag));
    } else if (tag.Type == "EnumProperty") {
        property = std::make_unique<EnumProperty>(std::move(tag));
    } else if (tag.Type == "FloatProperty") {
        property = std::make_unique<FloatProperty>(std::move(tag));
    } else if (tag.Type == "Int64Property") {
        property = std::make_unique<Int64Property>(std::move(tag));
    } else if (tag.Type == "Int8Property") {
        property = std::make_unique<Int8Property>(std::move(tag));
    } else if (tag.Type == "IntProperty") {
        property = std::make_unique<IntProperty>(std::move(tag));
    } else if (tag.Type == "MapProperty") {
        property = std::make_unique<MapProperty>(std::move(tag));
    } else if (tag.Type == "MulticastSparseDelegateProperty") {
        property = std::make_unique<MulticastSparseDelegateProperty>(std::move(tag));
    } else if (tag.Type == "NameProperty") {
        property = std::make_unique<NameProperty>(std::move(tag));
    } else if (tag.Type == "ObjectProperty") {
        property = std::make_unique<ObjectProperty>(std::move(tag));
    } else if (tag.Type == "SetProperty") {
        property = std::make_unique<SetProperty>(std::move(tag), parentClassName);
    } else if (tag.Type == "StrProperty") {
        property = std::make_unique<StrProperty>(std::move(tag));
    } else if (tag.Type == "StructProperty") {
        property = std::make_unique<StructProperty>(std::move(tag));
    } else if (tag.Type == "TextProperty") {
        property = std::make_unique<TextProperty>(std::move(tag));
    } else {
        throw std::runtime_error("Unknown property type: " + tag.Type);
    }

    auto pos_before = ar.tell();
    ar << *property;
    auto pos_after = ar.tell();
    if (pos_after - pos_before != property->tag().Size) {
        throw std::runtime_error(
            std::string("Invalid Property size!\nName: ") + property->name() + "\nType: " + property->type());
    }

    return property;
}

Satisfactory3DMap::Property::Property(Satisfactory3DMap::PropertyTag tag) : tag_(std::move(tag)) {}
