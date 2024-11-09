#pragma once

#include "../UE/Core/Internationalization/Text.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API TextProperty final : public PropertyImpl<TextProperty, FText> {
    public:
        static constexpr std::string_view TypeName = "TextProperty";

        using PropertyImpl<TextProperty, FText>::PropertyImpl;
    };
} // namespace SatisfactorySave
