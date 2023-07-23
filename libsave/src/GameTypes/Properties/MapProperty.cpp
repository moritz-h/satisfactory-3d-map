#include "GameTypes/Properties/MapProperty.h"

#include "GameTypes/MapTypes/ByteMapTypeList.h"
#include "GameTypes/MapTypes/EnumMapTypeList.h"
#include "GameTypes/MapTypes/FloatMapTypeList.h"
#include "GameTypes/MapTypes/IntMapTypeList.h"
#include "GameTypes/MapTypes/NameMapTypeList.h"
#include "GameTypes/MapTypes/ObjectMapTypeList.h"
#include "GameTypes/MapTypes/StructMapTypeList.h"
#include "GameTypes/Properties/Base/PropertyVisitor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

SatisfactorySave::MapProperty::MapProperty(SatisfactorySave::PropertyTag tag, std::string parentClassName)
    : Property(std::move(tag)),
      parentClassName_(std::move(parentClassName)) {}

void SatisfactorySave::MapProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        // TODO unknown
        inAr.read_assert_zero<int32_t>();

        auto count = inAr.read<int32_t>();

        if (Tag.InnerType == "EnumProperty") {
            keys_ = std::make_unique<EnumMapTypeList>(Tag.InnerType);
        } else if (Tag.InnerType == "IntProperty") {
            keys_ = std::make_unique<IntMapTypeList>(Tag.InnerType);
        } else if (Tag.InnerType == "NameProperty") {
            keys_ = std::make_unique<NameMapTypeList>(Tag.InnerType);
        } else if (Tag.InnerType == "ObjectProperty") {
            keys_ = std::make_unique<ObjectMapTypeList>(Tag.InnerType);
        } else if (Tag.InnerType == "StructProperty") {
            keys_ = std::make_unique<StructMapTypeList>(Tag.InnerType, Tag.Name, parentClassName_, true);
        } else {
            throw std::runtime_error("Map key type \"" + Tag.InnerType + "\" not implemented!");
        }

        if (Tag.ValueType == "ByteProperty") {
            values_ = std::make_unique<ByteMapTypeList>(Tag.ValueType);
        } else if (Tag.ValueType == "FloatProperty") {
            values_ = std::make_unique<FloatMapTypeList>(Tag.ValueType);
        } else if (Tag.ValueType == "IntProperty") {
            values_ = std::make_unique<IntMapTypeList>(Tag.ValueType);
        } else if (Tag.ValueType == "StructProperty") {
            values_ = std::make_unique<StructMapTypeList>(Tag.ValueType, Tag.Name, parentClassName_);
        } else {
            throw std::runtime_error("Map value type \"" + Tag.ValueType + "\" not implemented!");
        }

        for (int32_t i = 0; i < count; i++) {
            keys_->serializeEntry(inAr, i);
            values_->serializeEntry(inAr, i);
        }
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr.write<int32_t>(0);

        auto count = keys_->listSize();
        if (count != values_->listSize()) {
            throw std::runtime_error("Invalid map size!");
        }
        outAr.write(static_cast<int32_t>(count));

        for (std::size_t i = 0; i < count; i++) {
            keys_->serializeEntry(outAr, i);
            values_->serializeEntry(outAr, i);
        }
    }
}

void SatisfactorySave::MapProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
