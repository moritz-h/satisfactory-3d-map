#ifndef SATISFACTORY3DMAP_PROPERTYTAG_H
#define SATISFACTORY3DMAP_PROPERTYTAG_H

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>

#include "../Guid.h"

namespace Satisfactory3DMap {

    struct PropertyTag {
        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Public/UObject/PropertyTag.h#L21-L33
        std::string Name;
        std::string Type;
        int32_t Size = 0;
        int32_t ArrayIndex = -1;
        std::string StructName;
        Guid StructGuid;
        uint8_t BoolVal = 0;
        std::string EnumName;
        std::string InnerType;
        std::string ValueType;
        uint8_t HasPropertyGuid = 0;
        Guid PropertyGuid;
        mutable std::streampos SizeOffset = -1;

        explicit PropertyTag(std::istream& stream);

        virtual void serialize(std::ostream& stream) const;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PROPERTYTAG_H
