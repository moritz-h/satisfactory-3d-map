#pragma once

#include "../UE/UObject/Name.h"
#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class EnumArray : public ArrayImpl<EnumArray, FName> {
    public:
        static constexpr std::string_view TypeName = "EnumProperty";
    };
} // namespace SatisfactorySave
