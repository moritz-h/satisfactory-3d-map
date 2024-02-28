#include "GameTypes/Properties/MapProperty.h"

#include "GameTypes/Properties/Base/PropertyVisitor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

void SatisfactorySave::MapProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        int32_t NumKeysToRemove = 0;
        ar << NumKeysToRemove;
        if (NumKeysToRemove != 0) {
            throw std::runtime_error("MapProperty: NumKeysToRemove != 0 not implemented!");
        }

        auto NumEntries = inAr.read<int32_t>();

        Keys = MapTypeList::create(keyType(), name(), inAr.ParentClassInfo.top(), true);
        Values = MapTypeList::create(valueType(), name(), inAr.ParentClassInfo.top(), false);

        Keys->resize(NumEntries);
        Values->resize(NumEntries);

        for (int32_t i = 0; i < NumEntries; i++) {
            Keys->serializeEntry(inAr, i);
            Values->serializeEntry(inAr, i);
        }
    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        outAr.write<int32_t>(0);

        auto NumEntries = Keys->size();
        if (NumEntries != Values->size()) {
            throw std::runtime_error("Invalid map size!");
        }
        outAr.write(static_cast<int32_t>(NumEntries));

        for (std::size_t i = 0; i < NumEntries; i++) {
            Keys->serializeEntry(outAr, i);
            Values->serializeEntry(outAr, i);
        }
    }
}

void SatisfactorySave::MapProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
