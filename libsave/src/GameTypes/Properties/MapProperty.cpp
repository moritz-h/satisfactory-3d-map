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

        keys_ = MapTypeList::create(Tag.InnerType, Tag.Name, parentClassName_, true);
        values_ = MapTypeList::create(Tag.ValueType, Tag.Name, parentClassName_, false);

        keys_->resize(count);
        values_->resize(count);

        for (int32_t i = 0; i < count; i++) {
            keys_->serializeEntry(inAr, i);
            values_->serializeEntry(inAr, i);
        }
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr.write<int32_t>(0);

        auto count = keys_->size();
        if (count != values_->size()) {
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
