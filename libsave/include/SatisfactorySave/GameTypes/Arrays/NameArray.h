#pragma once

#include "../UE/Core/UObject/NameTypes.h"
#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API NameArray final : public ArrayImpl<NameArray, FName> {
    public:
        static constexpr std::string_view TypeName = "NameProperty";
    };
} // namespace SatisfactorySave
