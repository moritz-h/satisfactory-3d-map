#pragma once

#include "../UE/Core/Math/Color.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API LinearColorStruct final : public StructImpl<LinearColorStruct, FLinearColor> {
    public:
        static constexpr std::string_view TypeName = "LinearColor";

        using StructImpl<LinearColorStruct, FLinearColor>::StructImpl;
    };
} // namespace SatisfactorySave
