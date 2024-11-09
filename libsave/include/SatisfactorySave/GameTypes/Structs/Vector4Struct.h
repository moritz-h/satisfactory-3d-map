#pragma once

#include "../UE/Core/Math/Vector4.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API Vector4Struct final : public StructImpl<Vector4Struct, FVector4> {
    public:
        static constexpr std::string_view TypeName = "Vector4";

        using StructImpl<Vector4Struct, FVector4>::StructImpl;
    };
} // namespace SatisfactorySave
