#pragma once

#include "../UE/UObject/Name.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API EnumProperty final : public PropertyImpl<EnumProperty, FName> {
    public:
        static constexpr std::string_view TypeName = "EnumProperty";

        using PropertyImpl<EnumProperty, FName>::PropertyImpl;

        [[nodiscard]] inline FName& enumName() {
            return tag_.EnumName;
        }
    };
} // namespace SatisfactorySave
