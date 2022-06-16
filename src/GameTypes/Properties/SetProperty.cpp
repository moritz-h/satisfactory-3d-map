#include "SetProperty.h"

#include <utility>

#include "PropertyVisitor.h"

Satisfactory3DMap::SetProperty::SetProperty(Satisfactory3DMap::PropertyTag tag, std::string parentClassName)
    : Property(std::move(tag)),
      parentClassName_(std::move(parentClassName)) {}

void Satisfactory3DMap::SetProperty::serialize(Archive& ar) {
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PropertySet.cpp#L298
    int32_t NumElementsToRemove = 0;
    ar << NumElementsToRemove;
    if (NumElementsToRemove != 0) {
        throw std::runtime_error("SetProperty: NumElementsToRemove != 0 not implemented!");
    }

    if (ar.isIArchive()) {
        set_ = Set::create(tag_.InnerType, tag_.Name, parentClassName_, ar);
    } else {
        ar << *set_;
    }
}

void Satisfactory3DMap::SetProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}
