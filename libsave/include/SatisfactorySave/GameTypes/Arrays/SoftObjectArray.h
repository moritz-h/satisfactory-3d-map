#pragma once

#include "../UE/CoreUObject/UObject/SoftObjectPath.h"
#include "Base/ArrayImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API SoftObjectArray final : public ArrayImpl<SoftObjectArray, FSoftObjectPath> {
    public:
        static constexpr std::string_view TypeName = "SoftObjectProperty";
    };
} // namespace SatisfactorySave
