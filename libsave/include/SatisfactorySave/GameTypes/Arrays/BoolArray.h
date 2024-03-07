#pragma once

#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API BoolArray final : public ArrayImpl<BoolArray, int8_t> {
    public:
        static constexpr std::string_view TypeName = "BoolProperty";
    };
} // namespace SatisfactorySave
