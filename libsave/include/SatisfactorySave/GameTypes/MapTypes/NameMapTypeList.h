#pragma once

#include "../UE/UObject/Name.h"
#include "Base/MapTypeListImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API NameMapTypeList final : public MapTypeListImpl<NameMapTypeList, FName> {
    public:
        static constexpr std::string_view TypeName = "NameProperty";
    };
} // namespace SatisfactorySave
