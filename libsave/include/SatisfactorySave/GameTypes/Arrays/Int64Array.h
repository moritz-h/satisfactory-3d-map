#pragma once

#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API Int64Array final : public ArrayImpl<Int64Array, int64_t> {
    public:
        static constexpr std::string_view TypeName = "Int64Property";
    };
} // namespace SatisfactorySave
