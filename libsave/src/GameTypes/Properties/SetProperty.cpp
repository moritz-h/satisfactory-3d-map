#include "GameTypes/Properties/SetProperty.h"

#include <utility>

#include "GameTypes/Properties/Base/PropertyVisitor.h"

SatisfactorySave::SetProperty::SetProperty(SatisfactorySave::PropertyTag tag, std::string parentClassName)
    : Property(std::move(tag)),
      parentClassName_(std::move(parentClassName)) {}

void SatisfactorySave::SetProperty::serialize(Archive& ar) {
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PropertySet.cpp#L298
    int32_t NumElementsToRemove = 0;
    ar << NumElementsToRemove;
    if (NumElementsToRemove != 0) {
        throw std::runtime_error("SetProperty: NumElementsToRemove != 0 not implemented!");
    }

    if (ar.isIArchive()) {
        set_ = Set::create(setType(), name(), parentClassName_, ar);
    } else {
        ar << *set_;
    }
}

void SatisfactorySave::SetProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
