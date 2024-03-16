#include "GameTypes/Properties/Base/Property.h"

#include <utility>

#include <spdlog/spdlog.h>

#include "GameTypes/Properties/Base/PropertyAll.h"
#include "GameTypes/Properties/Base/PropertyTag.h"

std::unique_ptr<SatisfactorySave::Property> SatisfactorySave::Property::create(IStreamArchive& ar) {
    PropertyTag tag;
    ar << tag;

    if (tag.Name == "None") {
        return nullptr;
    }

    std::unique_ptr<Property> property;

    if (tag.Type == ArrayProperty::TypeName) {
        property = std::make_unique<ArrayProperty>(std::move(tag));
    } else if (tag.Type == BoolProperty::TypeName) {
        property = std::make_unique<BoolProperty>(std::move(tag));
    } else if (tag.Type == ByteProperty::TypeName) {
        property = std::make_unique<ByteProperty>(std::move(tag));
    } else if (tag.Type == DoubleProperty::TypeName) {
        property = std::make_unique<DoubleProperty>(std::move(tag));
    } else if (tag.Type == EnumProperty::TypeName) {
        property = std::make_unique<EnumProperty>(std::move(tag));
    } else if (tag.Type == FloatProperty::TypeName) {
        property = std::make_unique<FloatProperty>(std::move(tag));
    } else if (tag.Type == Int64Property::TypeName) {
        property = std::make_unique<Int64Property>(std::move(tag));
    } else if (tag.Type == Int8Property::TypeName) {
        property = std::make_unique<Int8Property>(std::move(tag));
    } else if (tag.Type == IntProperty::TypeName) {
        property = std::make_unique<IntProperty>(std::move(tag));
    } else if (tag.Type == MapProperty::TypeName) {
        property = std::make_unique<MapProperty>(std::move(tag));
    } else if (tag.Type == MulticastSparseDelegateProperty::TypeName) {
        property = std::make_unique<MulticastSparseDelegateProperty>(std::move(tag));
    } else if (tag.Type == NameProperty::TypeName) {
        property = std::make_unique<NameProperty>(std::move(tag));
    } else if (tag.Type == ObjectProperty::TypeName) {
        property = std::make_unique<ObjectProperty>(std::move(tag));
    } else if (tag.Type == SetProperty::TypeName) {
        property = std::make_unique<SetProperty>(std::move(tag));
    } else if (tag.Type == SoftObjectProperty::TypeName) {
        property = std::make_unique<SoftObjectProperty>(std::move(tag));
    } else if (tag.Type == StrProperty::TypeName) {
        property = std::make_unique<StrProperty>(std::move(tag));
    } else if (tag.Type == StructProperty::TypeName) {
        property = std::make_unique<StructProperty>(std::move(tag));
    } else if (tag.Type == TextProperty::TypeName) {
        property = std::make_unique<TextProperty>(std::move(tag));
    } else if (tag.Type == UInt32Property::TypeName) {
        property = std::make_unique<UInt32Property>(std::move(tag));
    } else if (tag.Type == UInt64Property::TypeName) {
        property = std::make_unique<UInt64Property>(std::move(tag));
    } else {
        spdlog::warn("Unknown property type: {}", tag.Type.toString());
        property = std::make_unique<UnknownProperty>(std::move(tag));
    }

    auto pos_before = ar.tell();
    ar.pushReadLimit(property->tag_.Size);
    static int recursion_depth = 0; // Count recursion depth for better debug logging.
    recursion_depth++;
    try {
        ar << *property;
        recursion_depth--;
    } catch (const std::exception& ex) {
        recursion_depth--;
        PropertyTag tagCopy = property->tag_;
        spdlog::error("Error parsing property {} (Type: {}, Class: {}) in recursion depth {}: {}",
            tagCopy.Name.toString(), tagCopy.Type.toString(), ar.ParentClassInfo.top(), recursion_depth, ex.what());

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
    if (pos_after - pos_before != property->tag_.Size) {
        throw std::runtime_error(
            std::string("Invalid Property size!\nName: ") + property->tag_.Name + "\nType: " + property->tag_.Type);
    }

    return property;
}

SatisfactorySave::Property::Property(FName type) {
    tag_.Type = std::move(type);
}

SatisfactorySave::Property::Property(FName type, FName name) {
    tag_.Type = std::move(type);
    tag_.Name = std::move(name);
}

SatisfactorySave::Property::Property(PropertyTag tag) : tag_(std::move(tag)) {}
