#pragma once

#include "../UE/Core/UObject/NameTypes.h"
#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API EnumArray final : public ArrayImpl<EnumArray, FName> {
    public:
        static constexpr std::string_view TypeName = "EnumProperty";
    };
} // namespace SatisfactorySave
