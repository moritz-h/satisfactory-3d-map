#pragma once

#include <string>

#include "Base/MapTypeListImpl.h"

namespace SatisfactorySave {

    class EnumMapTypeList : public MapTypeListImpl<EnumMapTypeList, std::string> {
    public:
        static constexpr std::string_view TypeName = "EnumProperty";
    };
} // namespace SatisfactorySave
