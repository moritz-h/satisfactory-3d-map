#pragma once

#include "../UE/Core/Math/Color.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ColorStruct final : public StructImpl<ColorStruct, FColor> {
    public:
        static constexpr std::string_view TypeName = "Color";

        using StructImpl<ColorStruct, FColor>::StructImpl;
    };
} // namespace SatisfactorySave
