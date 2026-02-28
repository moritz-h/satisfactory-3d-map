#pragma once

#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "../../Core/Misc/Guid.h"
#include "../../Core/UObject/NameTypes.h"
#include "PropertyTypeName.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    enum class EPropertyTagSerializeType : uint8_t {
        Unknown,
        Skipped,
        Property,
        BinaryOrNative,
    };

    struct SATISFACTORYSAVE_API FPropertyTag {
        FPropertyTypeName_Data TypeName;
        FName Name;
        FName Type;
        int32_t Size = 0;
        int32_t ArrayIndex = -1;
        FName StructName;
        FGuid StructGuid;
        uint8_t BoolVal = 0;
        FName EnumName;
        FName InnerType;
        FName ValueType;
        uint8_t HasPropertyGuid = 0;
        FGuid PropertyGuid;
        EPropertyTagSerializeType SerializeType = EPropertyTagSerializeType::Unknown;
        std::size_t SizeOffset = static_cast<std::size_t>(-1);

        FPropertyTag() = default;

        void serialize(Archive& ar);

    protected:
        void SetType();
    };
} // namespace SatisfactorySave
