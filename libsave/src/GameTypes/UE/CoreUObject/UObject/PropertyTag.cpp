#include "GameTypes/UE/CoreUObject/UObject/PropertyTag.h"

enum class EPropertyTagFlags : uint8_t {
    None = 0x00,
    HasArrayIndex = 0x01,
    HasPropertyGuid = 0x02,
    HasPropertyExtensions = 0x04,
    HasBinaryOrNativeSerialize = 0x08,
    BoolTrue = 0x10,
    SkippedSerialize = 0x20,
};

void SatisfactorySave::PropertyTag::serialize(Archive& ar) {
    ar << Name;
    if (Name == "None") {
        return;
    }

    if (ar.getSaveVersion() >= 53) {
        ar << TypeName;
        if (ar.isIArchive()) {
            SetType();
        }

        if (ar.isOArchive()) {
            // Store position to write size, after object was serialized.
            SizeOffset = ar.tell();
        }
        ar << Size;

        int8_t PropertyTagFlags = 0;

        if (ar.isOArchive()) {
            if (ArrayIndex != 0) {
                PropertyTagFlags |= 0x01;
            }
            if (HasPropertyGuid) {
                PropertyTagFlags = 0x02;
            }
            if (SerializeType == EPropertyTagSerializeType::BinaryOrNative) {
                PropertyTagFlags |= 0x08;
            }
            if (SerializeType == EPropertyTagSerializeType::Skipped) {
                PropertyTagFlags |= 0x20;
            }
            if (BoolVal && Type == "BoolProperty") {
                PropertyTagFlags |= 0x10;
            }
        }

        ar << PropertyTagFlags;

        if (ar.isIArchive()) {
            HasPropertyGuid = PropertyTagFlags & 0x02;
            SerializeType = (PropertyTagFlags & 0x20)
                                ? EPropertyTagSerializeType::Skipped
                                : ((PropertyTagFlags & 0x08) ? EPropertyTagSerializeType::BinaryOrNative
                                                             : EPropertyTagSerializeType::Property);
            BoolVal = PropertyTagFlags & 0x10;
        }

        if (PropertyTagFlags & 0x01) {
            ar << ArrayIndex;
        } else {
            ArrayIndex = 0;
        }

        if (PropertyTagFlags & 0x02) {
            ar << PropertyGuid;
        }

        if (PropertyTagFlags & 0x04) {
            throw std::runtime_error("PropertyTag: PropertyExtensions not implemented!");
        }
    } else {
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
}

void SatisfactorySave::PropertyTag::SetType() {
    Type = TypeName.GetName();

    if (Type.Number == /*NAME_NO_NUMBER_INTERNAL*/ 0) {
        if (Type.Name == "StructProperty") {
            StructName = TypeName.GetParameterName(0);
            FName StructGuidName = TypeName.GetParameterName(1);
            if (StructGuidName != "None") {
                throw std::runtime_error("PropertyTag: Parsing StructGuidName not implemented!");
            }
        } else if (Type.Name == "ByteProperty" || Type.Name == "EnumProperty") {
            EnumName = TypeName.GetParameterName(0);
        } else if (Type.Name == "ArrayProperty" || Type.Name == "OptionalProperty" || Type.Name == "SetProperty") {
            InnerType = TypeName.GetParameterName(0);
        } else if (Type.Name == "MapProperty") {
            InnerType = TypeName.GetParameterName(0);
            ValueType = TypeName.GetParameterName(1);
        }
    }
}
