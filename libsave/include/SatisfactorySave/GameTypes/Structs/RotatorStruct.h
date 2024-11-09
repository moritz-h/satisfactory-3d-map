#pragma once

#include "../UE/Core/Math/Rotator.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API RotatorStruct final : public StructImpl<RotatorStruct, FRotator> {
    public:
        static constexpr std::string_view TypeName = "Rotator";

        using StructImpl<RotatorStruct, FRotator>::StructImpl;
    };
} // namespace SatisfactorySave
