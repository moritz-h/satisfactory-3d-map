#include "MapProperty.h"

#include "../MapTypes/ByteMapTypeList.h"
#include "../MapTypes/EnumMapTypeList.h"
#include "../MapTypes/IntMapTypeList.h"
#include "../MapTypes/ObjectMapTypeList.h"
#include "../MapTypes/StructMapTypeList.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"
#include "PropertyVisitor.h"

void Satisfactory3DMap::MapProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        // TODO unknown
        inAr.read_assert_zero<int32_t>();

        auto count = inAr.read<int32_t>();

        if (tag_.InnerType == "EnumProperty") {
            keys_ = std::make_unique<EnumMapTypeList>(tag_.InnerType);
        } else if (tag_.InnerType == "IntProperty") {
            keys_ = std::make_unique<IntMapTypeList>(tag_.InnerType);
        } else if (tag_.InnerType == "ObjectProperty") {
            keys_ = std::make_unique<ObjectMapTypeList>(tag_.InnerType);
        } else {
            throw std::runtime_error("Map key type \"" + tag_.InnerType + "\" not implemented!");
        }

        if (tag_.ValueType == "ByteProperty") {
            values_ = std::make_unique<ByteMapTypeList>(tag_.ValueType);
        } else if (tag_.ValueType == "IntProperty") {
            values_ = std::make_unique<IntMapTypeList>(tag_.ValueType);
        } else if (tag_.ValueType == "StructProperty") {
            values_ = std::make_unique<StructMapTypeList>(tag_.ValueType);
        } else {
            throw std::runtime_error("Map value type \"" + tag_.ValueType + "\" not implemented!");
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

void Satisfactory3DMap::MapProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
