#pragma once

#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API Int8Array final : public ArrayImpl<Int8Array, int8_t> {
    public:
        static constexpr std::string_view TypeName = "Int8Property";
    };
} // namespace SatisfactorySave
