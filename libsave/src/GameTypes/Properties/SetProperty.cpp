#include "GameTypes/Properties/SetProperty.h"

void SatisfactorySave::SetProperty::serialize(Archive& ar) {
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PropertySet.cpp#L298
    int32_t NumElementsToRemove = 0;
    ar << NumElementsToRemove;
    if (NumElementsToRemove != 0) {
        throw std::runtime_error("SetProperty: NumElementsToRemove != 0 not implemented!");
    }

    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);
        Value = Set::create(SetType(), Name(), inAr);
    } else {
        ar << *Value;
    }
}
