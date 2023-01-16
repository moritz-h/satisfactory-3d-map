#include "SoftObjectProperty.h"

#include "PropertyVisitor.h"

// Serialization starts here:
// https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PropertySoftObjectPtr.cpp#L46-L75
// Following the serialization path seems to end here:
// https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/SoftObjectPath.cpp#L244-L245
void Satisfactory3DMap::SoftObjectProperty::serialize(Archive& ar) {
    ar << value_;
}

void Satisfactory3DMap::SoftObjectProperty::accept(Satisfactory3DMap::PropertyVisitor& v) {
    v.visit(*this);
}