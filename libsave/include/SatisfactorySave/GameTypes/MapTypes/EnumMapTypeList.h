#pragma once

#include "../UE/UObject/Name.h"
#include "Base/MapTypeListImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API EnumMapTypeList final : public MapTypeListImpl<EnumMapTypeList, FName> {
    public:
        static constexpr std::string_view TypeName = "EnumProperty";
    };
} // namespace SatisfactorySave
