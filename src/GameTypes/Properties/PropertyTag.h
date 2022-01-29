#ifndef SATISFACTORY3DMAP_PROPERTYTAG_H
#define SATISFACTORY3DMAP_PROPERTYTAG_H

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>

#include "../Guid.h"
#include "../Name.h"
#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    struct PropertyTag {
        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Public/UObject/PropertyTag.h#L21-L33
        FName Name;
        FName Type;
        int32_t Size = 0;
        int32_t ArrayIndex = -1;
        FName StructName;
        Guid StructGuid;
        uint8_t BoolVal = 0;
        FName EnumName;
        FName InnerType;
        FName ValueType;
        uint8_t HasPropertyGuid = 0;
        Guid PropertyGuid;
        std::size_t SizeOffset = -1;

        explicit PropertyTag() = default;

        virtual void serialize(Archive& ar);
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTYTAG_H
