#pragma once

#include "Base/SetImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API UInt32Set final : public SetImpl<UInt32Set, uint32_t> {
    public:
        static constexpr std::string_view TypeName = "UInt32Property";
    };
} // namespace SatisfactorySave
