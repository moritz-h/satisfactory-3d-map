#include "PropertyTag.h"

#include "Utils/StreamUtils.h"

Satisfactory3DMap::PropertyTag::PropertyTag(std::istream& stream) {
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PropertyTag.cpp#L81-L205

    Name = read_length_string(stream);
    if (Name == "None") {
        return;
    }

    Type = read_length_string(stream);
    Size = read<int32_t>(stream);
    ArrayIndex = read<int32_t>(stream);

    if (Type == "StructProperty") {
        StructName = read_length_string(stream);
        StructGuid = Guid(stream);
    } else if (Type == "BoolProperty") {
        BoolVal = read<int8_t>(stream);
    } else if (Type == "ByteProperty" || Type == "EnumProperty") {
        EnumName = read_length_string(stream);
    } else if (Type == "ArrayProperty" || Type == "SetProperty") {
        InnerType = read_length_string(stream);
    } else if (Type == "MapProperty") {
        InnerType = read_length_string(stream);
        ValueType = read_length_string(stream);
    }

    HasPropertyGuid = read<uint8_t>(stream);
    if (HasPropertyGuid) {
        PropertyGuid = Guid(stream);
    }
}

void Satisfactory3DMap::PropertyTag::serialize(std::ostream& stream) const {
    write_length_string(stream, Name);
    if (Name == "None") {
        return;
    }

    write_length_string(stream, Type);
    SizeOffset = stream.tellp();
    write(stream, Size);
    write(stream, ArrayIndex);

    if (Type == "StructProperty") {
        write_length_string(stream, StructName);
        StructGuid.serialize(stream);
    } else if (Type == "BoolProperty") {
        write(stream, BoolVal);
    } else if (Type == "ByteProperty" || Type == "EnumProperty") {
        write_length_string(stream, EnumName);
    } else if (Type == "ArrayProperty" || Type == "SetProperty") {
        write_length_string(stream, InnerType);
    } else if (Type == "MapProperty") {
        write_length_string(stream, InnerType);
        write_length_string(stream, ValueType);
    }

    write(stream, HasPropertyGuid);
    if (HasPropertyGuid) {
        PropertyGuid.serialize(stream);
    }
}
