#pragma once

#include "Base/MapTypeListImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API Int64MapTypeList final : public MapTypeListImpl<Int64MapTypeList, int64_t> {
    public:
        static constexpr std::string_view TypeName = "Int64Property";
    };
} // namespace SatisfactorySave
