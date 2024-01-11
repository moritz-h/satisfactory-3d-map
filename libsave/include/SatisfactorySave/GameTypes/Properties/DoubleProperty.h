#pragma once

#include "Base/PropertyImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API DoubleProperty : public PropertyImpl<DoubleProperty, double> {
    public:
        static constexpr std::string_view TypeName = "DoubleProperty";

        using PropertyImpl<DoubleProperty, double>::PropertyImpl;
    };
} // namespace SatisfactorySave
