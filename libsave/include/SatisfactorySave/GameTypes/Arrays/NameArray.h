#pragma once

#include "../Misc/Name.h"
#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class NameArray : public ArrayImpl<NameArray, FName> {
    public:
        static constexpr std::string_view TypeName = "NameProperty";
    };
} // namespace SatisfactorySave
