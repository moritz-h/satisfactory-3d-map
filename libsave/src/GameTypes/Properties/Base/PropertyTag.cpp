#include "GameTypes/Properties/Base/PropertyTag.h"

void SatisfactorySave::PropertyTag::serialize(Archive& ar) {
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PropertyTag.cpp#L81-L205

    ar << Name;
    if (Name == "None") {
        return;
    }

    ar << Type;

    if (ar.isOArchive()) {
        // Store position to write size, after object was serialized.
        SizeOffset = ar.tell();
    }
    ar << Size;
    ar << ArrayIndex;

    if (Type == "StructProperty") {
        ar << StructName;
        ar << StructGuid;
    } else if (Type == "BoolProperty") {
        ar << BoolVal;
    } else if (Type == "ByteProperty" || Type == "EnumProperty") {
        ar << EnumName;
    } else if (Type == "ArrayProperty" || Type == "SetProperty") {
        ar << InnerType;
    } else if (Type == "MapProperty") {
        ar << InnerType;
        ar << ValueType;
    }

    ar << HasPropertyGuid;
    if (HasPropertyGuid) {
        ar << PropertyGuid;
    }
}
