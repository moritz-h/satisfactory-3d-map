#pragma once

#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class FloatArray : public ArrayImpl<FloatArray, float> {
    public:
        static constexpr std::string_view TypeName = "FloatProperty";
    };
} // namespace SatisfactorySave
