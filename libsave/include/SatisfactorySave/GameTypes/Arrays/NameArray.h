#pragma once

#include "../UE/UObject/Name.h"
#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API NameArray final : public ArrayImpl<NameArray, FName> {
    public:
        static constexpr std::string_view TypeName = "NameProperty";
    };
} // namespace SatisfactorySave
