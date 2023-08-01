#pragma once

#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class FloatProperty : public PropertyImpl<FloatProperty, float> {
    public:
        static constexpr std::string_view TypeName = "FloatProperty";

        using PropertyImpl<FloatProperty, float>::PropertyImpl;
    };
} // namespace SatisfactorySave
