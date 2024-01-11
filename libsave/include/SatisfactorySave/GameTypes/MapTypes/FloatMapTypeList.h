#pragma once

#include "Base/MapTypeListImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API FloatMapTypeList : public MapTypeListImpl<FloatMapTypeList, float> {
    public:
        static constexpr std::string_view TypeName = "FloatProperty";
    };
} // namespace SatisfactorySave
