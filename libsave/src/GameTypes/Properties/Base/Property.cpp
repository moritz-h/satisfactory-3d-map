#include "GameTypes/Properties/Property.h"

#include <utility>

#include <spdlog/spdlog.h>

#include "GameTypes/Properties/ArrayProperty.h"
#include "GameTypes/Properties/BoolProperty.h"
#include "GameTypes/Properties/ByteProperty.h"
#include "GameTypes/Properties/DoubleProperty.h"
#include "GameTypes/Properties/EnumProperty.h"
#include "GameTypes/Properties/FloatProperty.h"
#include "GameTypes/Properties/Int64Property.h"
#include "GameTypes/Properties/Int8Property.h"
#include "GameTypes/Properties/IntProperty.h"
#include "GameTypes/Properties/MapProperty.h"
#include "GameTypes/Properties/MulticastSparseDelegateProperty.h"
#include "GameTypes/Properties/NameProperty.h"
#include "GameTypes/Properties/ObjectProperty.h"
#include "GameTypes/Properties/PropertyTag.h"
#include "GameTypes/Properties/SetProperty.h"
#include "GameTypes/Properties/SoftObjectProperty.h"
#include "GameTypes/Properties/StrProperty.h"
#include "GameTypes/Properties/StructProperty.h"
#include "GameTypes/Properties/TextProperty.h"
#include "GameTypes/Properties/UInt32Property.h"
#include "GameTypes/Properties/UInt64Property.h"
#include "GameTypes/Properties/UnknownProperty.h"

std::unique_ptr<SatisfactorySave::Property> SatisfactorySave::Property::create(SatisfactorySave::IStreamArchive& ar,
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
    } else if (tag.Type == "DoubleProperty") {
        property = std::make_unique<DoubleProperty>(std::move(tag));
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
        property = std::make_unique<MapProperty>(std::move(tag), parentClassName);
    } else if (tag.Type == "MulticastSparseDelegateProperty") {
        property = std::make_unique<MulticastSparseDelegateProperty>(std::move(tag));
    } else if (tag.Type == "NameProperty") {
        property = std::make_unique<NameProperty>(std::move(tag));
    } else if (tag.Type == "ObjectProperty") {
        property = std::make_unique<ObjectProperty>(std::move(tag));
    } else if (tag.Type == "SetProperty") {
        property = std::make_unique<SetProperty>(std::move(tag), parentClassName);
    } else if (tag.Type == "SoftObjectProperty") {
        property = std::make_unique<SoftObjectProperty>(std::move(tag));
    } else if (tag.Type == "StrProperty") {
        property = std::make_unique<StrProperty>(std::move(tag));
    } else if (tag.Type == "StructProperty") {
        property = std::make_unique<StructProperty>(std::move(tag));
    } else if (tag.Type == "TextProperty") {
        property = std::make_unique<TextProperty>(std::move(tag));
    } else if (tag.Type == "UInt32Property") {
        property = std::make_unique<UInt32Property>(std::move(tag));
    } else if (tag.Type == "UInt64Property") {
        property = std::make_unique<UInt64Property>(std::move(tag));
    } else {
        spdlog::warn("Unknown property type: {}", tag.Type.toString());
        property = std::make_unique<UnknownProperty>(std::move(tag));
    }

    auto pos_before = ar.tell();
    ar.pushReadLimit(property->tag().Size);
    static int recursion_depth = 0; // Count recursion depth for better debug logging.
    recursion_depth++;
    try {
        ar << *property;
        recursion_depth--;
    } catch (const std::exception& ex) {
        recursion_depth--;
        PropertyTag tagCopy = property->tag();
        spdlog::error("Error parsing property {} (Type: {}, Class: {}) in recursion depth {}: {}",
            tagCopy.Name.toString(), tagCopy.Type.toString(), parentClassName, recursion_depth, ex.what());

        // Reset stream
        ar.rawStream().clear();
        ar.seek(pos_before);

        try {
            property = std::make_unique<UnknownProperty>(std::move(tagCopy));
            ar << *property;
            spdlog::info("Could read as unknown property!");
        } catch (const std::exception& ex) {
            spdlog::error("Could not parse as unknown property: {}", ex.what());
            ar.popReadLimit();
            throw;
        }
    }
    ar.popReadLimit();
    auto pos_after = ar.tell();
    if (pos_after - pos_before != property->tag().Size) {
        throw std::runtime_error(
            std::string("Invalid Property size!\nName: ") + property->name() + "\nType: " + property->type());
    }

    return property;
}

SatisfactorySave::Property::Property(SatisfactorySave::PropertyTag tag) : tag_(std::move(tag)) {}
