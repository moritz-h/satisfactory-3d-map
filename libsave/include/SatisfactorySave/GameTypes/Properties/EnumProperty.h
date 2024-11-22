#pragma once

#include "../UE/Core/UObject/NameTypes.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API EnumProperty final : public PropertyImpl<EnumProperty, FName> {
    public:
        static constexpr std::string_view TypeName = "EnumProperty";

        using PropertyImpl<EnumProperty, FName>::PropertyImpl;

        [[nodiscard]] inline FName& EnumName() {
            return tag_.EnumName;
        }
    };
} // namespace SatisfactorySave
