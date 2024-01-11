#pragma once

#include <string>

#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API StrProperty : public PropertyImpl<StrProperty, std::string> {
    public:
        static constexpr std::string_view TypeName = "StrProperty";

        using PropertyImpl<StrProperty, std::string>::PropertyImpl;
    };
} // namespace SatisfactorySave
