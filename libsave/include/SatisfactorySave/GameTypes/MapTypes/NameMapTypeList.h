#pragma once

#include "../UE/UObject/Name.h"
#include "Base/MapTypeListImpl.h"

namespace SatisfactorySave {

    class NameMapTypeList : public MapTypeListImpl<NameMapTypeList, FName> {
    public:
        static constexpr std::string_view TypeName = "NameProperty";
    };
} // namespace SatisfactorySave
