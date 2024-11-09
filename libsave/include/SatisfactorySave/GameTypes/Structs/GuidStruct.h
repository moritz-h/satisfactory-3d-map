#pragma once

#include "../UE/Core/Misc/Guid.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API GuidStruct final : public StructImpl<GuidStruct, FGuid> {
    public:
        static constexpr std::string_view TypeName = "Guid";

        using StructImpl<GuidStruct, FGuid>::StructImpl;
    };
} // namespace SatisfactorySave
