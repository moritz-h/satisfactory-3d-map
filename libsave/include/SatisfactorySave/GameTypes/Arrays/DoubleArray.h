#pragma once

#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API DoubleArray final : public ArrayImpl<DoubleArray, double> {
    public:
        static constexpr std::string_view TypeName = "DoubleProperty";
    };
} // namespace SatisfactorySave
