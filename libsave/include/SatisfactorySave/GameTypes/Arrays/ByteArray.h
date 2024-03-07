#pragma once

#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ByteArray final : public ArrayImpl<ByteArray, int8_t> {
    public:
        static constexpr std::string_view TypeName = "ByteProperty";
    };
} // namespace SatisfactorySave
