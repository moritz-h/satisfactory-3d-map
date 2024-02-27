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

void SatisfactorySave::MapProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        // TODO unknown
        inAr.read_assert_zero<int32_t>();

        auto count = inAr.read<int32_t>();

        Keys = MapTypeList::create(keyType(), name(), inAr.ParentClassInfo.top(), true);
        Values = MapTypeList::create(valueType(), name(), inAr.ParentClassInfo.top(), false);

        Keys->resize(count);
        Values->resize(count);

        for (int32_t i = 0; i < count; i++) {
            Keys->serializeEntry(inAr, i);
            Values->serializeEntry(inAr, i);
        }
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr.write<int32_t>(0);

        auto count = Keys->size();
        if (count != Values->size()) {
            throw std::runtime_error("Invalid map size!");
        }
        outAr.write(static_cast<int32_t>(count));

        for (std::size_t i = 0; i < count; i++) {
            Keys->serializeEntry(outAr, i);
            Values->serializeEntry(outAr, i);
        }
    }
}

void SatisfactorySave::MapProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
