#pragma once

#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API IntProperty final : public PropertyImpl<IntProperty, int32_t> {
    public:
        static constexpr std::string_view TypeName = "IntProperty";

        using PropertyImpl<IntProperty, int32_t>::PropertyImpl;
    };
} // namespace SatisfactorySave
