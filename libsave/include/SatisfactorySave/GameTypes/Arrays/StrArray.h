#pragma once

#include <string>

#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API StrArray final : public ArrayImpl<StrArray, std::string> {
    public:
        static constexpr std::string_view TypeName = "StrProperty";
    };
} // namespace SatisfactorySave
