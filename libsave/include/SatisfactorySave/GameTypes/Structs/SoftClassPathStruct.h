#pragma once

#include "../UE/UObject/SoftObjectPath.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API SoftClassPathStruct : public StructImpl<SoftClassPathStruct, FSoftObjectPath> {
    public:
        static constexpr std::string_view TypeName = "SoftClassPath";

        using StructImpl<SoftClassPathStruct, FSoftObjectPath>::StructImpl;
    };
} // namespace SatisfactorySave
