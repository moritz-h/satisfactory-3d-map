#pragma once

#include "../UE/Core/UObject/NameTypes.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API NameProperty final : public PropertyImpl<NameProperty, FName> {
    public:
        static constexpr std::string_view TypeName = "NameProperty";

        using PropertyImpl<NameProperty, FName>::PropertyImpl;
    };
} // namespace SatisfactorySave
