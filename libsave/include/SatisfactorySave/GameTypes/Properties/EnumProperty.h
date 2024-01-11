#pragma once

#include "../UE/UObject/Name.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API EnumProperty : public PropertyImpl<EnumProperty, FName> {
    public:
        static constexpr std::string_view TypeName = "EnumProperty";

        using PropertyImpl<EnumProperty, FName>::PropertyImpl;

        [[nodiscard]] const FName& enumType() const {
            return Tag.EnumName;
        }
    };
} // namespace SatisfactorySave
