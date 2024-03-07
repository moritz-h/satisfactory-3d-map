#pragma once

#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API FloatProperty final : public PropertyImpl<FloatProperty, float> {
    public:
        static constexpr std::string_view TypeName = "FloatProperty";

        using PropertyImpl<FloatProperty, float>::PropertyImpl;
    };
} // namespace SatisfactorySave
