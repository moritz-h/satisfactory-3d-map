#include "GameTypes/Properties/MapProperty.h"

#include "GameTypes/Properties/Base/PropertyVisitor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

SatisfactorySave::MapProperty::MapProperty(FName name, std::shared_ptr<MapTypeList> keys,
    std::shared_ptr<MapTypeList> values)
    : Property(FName(std::string(TypeName)), std::move(name)) {
    Keys = std::move(keys);
    Values = std::move(values);
}

SatisfactorySave::MapProperty::MapProperty(const MapProperty& other) : Property(other) {
    if (other.Keys) {
        Keys = other.Keys->clone();
    }
    if (other.Values) {
        Values = other.Values->clone();
    }
}

SatisfactorySave::MapProperty& SatisfactorySave::MapProperty::operator=(const MapProperty& other) {
    if (this != &other) {
        Property::operator=(other);
        if (other.Keys) {
            Keys = other.Keys->clone();
        } else {
            Keys.reset();
        }
        if (other.Values) {
            Values = other.Values->clone();
        } else {
            Values.reset();
        }
    }
    return *this;
}

std::shared_ptr<SatisfactorySave::Property> SatisfactorySave::MapProperty::clone() const {
    return std::make_shared<MapProperty>(*this);
}

void SatisfactorySave::MapProperty::serialize(Archive& ar) {
    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        int32_t NumKeysToRemove = 0;
        ar << NumKeysToRemove;
        if (NumKeysToRemove != 0) {
            throw std::runtime_error("MapProperty: NumKeysToRemove != 0 not implemented!");
        }

        auto NumEntries = inAr.read<int32_t>();

        Keys = MapTypeList::create(KeyType(), Name(), inAr.getParentClassInfo(), true);
        Values = MapTypeList::create(ValueType(), Name(), inAr.getParentClassInfo(), false);

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

void SatisfactorySave::MapProperty::accept(PropertyVisitor& v) {
    v.visit(*this);
}
