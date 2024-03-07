#pragma once

#include "Base/MapTypeListImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API IntMapTypeList final : public MapTypeListImpl<IntMapTypeList, int32_t> {
    public:
        static constexpr std::string_view TypeName = "IntProperty";
    };
} // namespace SatisfactorySave
