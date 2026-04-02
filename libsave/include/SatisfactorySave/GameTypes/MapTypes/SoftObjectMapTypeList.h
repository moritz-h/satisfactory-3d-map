#pragma once

#include "../UE/CoreUObject/UObject/SoftObjectPath.h"
#include "Base/MapTypeListImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API SoftObjectMapTypeList final
        : public MapTypeListImpl<SoftObjectMapTypeList, FSoftObjectPath> {
    public:
        static constexpr std::string_view TypeName = "SoftObjectProperty";
    };
} // namespace SatisfactorySave
