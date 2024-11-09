#pragma once

#include "../UE/CoreUObject/UObject/SoftObjectPath.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API SoftClassPathStruct final : public StructImpl<SoftClassPathStruct, FSoftObjectPath> {
    public:
        static constexpr std::string_view TypeName = "SoftClassPath";

        using StructImpl<SoftClassPathStruct, FSoftObjectPath>::StructImpl;
    };
} // namespace SatisfactorySave
