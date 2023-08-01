#include "GameTypes/Properties/SoftObjectProperty.h"

#include "GameTypes/Properties/Base/PropertyVisitor.h"

// Serialization starts here:
// https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PropertySoftObjectPtr.cpp#L46-L75
// Following the serialization path seems to end here:
// https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/SoftObjectPath.cpp#L244-L245
void SatisfactorySave::SoftObjectProperty::serialize(Archive& ar) {
    ar << value_;
}

void SatisfactorySave::SoftObjectProperty::accept(SatisfactorySave::PropertyVisitor& v) {
    v.visit(*this);
}
