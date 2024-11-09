#pragma once

#include "../UE/Core/Math/Quat.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API QuatStruct final : public StructImpl<QuatStruct, FQuat> {
    public:
        static constexpr std::string_view TypeName = "Quat";

        using StructImpl<QuatStruct, FQuat>::StructImpl;
    };
} // namespace SatisfactorySave
