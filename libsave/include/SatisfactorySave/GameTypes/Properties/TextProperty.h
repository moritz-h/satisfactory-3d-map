#pragma once

#include "../UE/Internationalization/Text.h"
#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class TextProperty : public PropertyImpl<TextProperty, FText> {
    public:
        static constexpr std::string_view TypeName = "TextProperty";

        using PropertyImpl<TextProperty, FText>::PropertyImpl;
    };
} // namespace SatisfactorySave
