#pragma once

#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API FloatArray final : public ArrayImpl<FloatArray, float> {
    public:
        static constexpr std::string_view TypeName = "FloatProperty";
    };
} // namespace SatisfactorySave
