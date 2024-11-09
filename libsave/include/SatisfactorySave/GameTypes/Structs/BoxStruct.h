#pragma once

#include "../UE/Core/Math/Box.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API BoxStruct final : public StructImpl<BoxStruct, FBox> {
    public:
        static constexpr std::string_view TypeName = "Box";

        using StructImpl<BoxStruct, FBox>::StructImpl;
    };
} // namespace SatisfactorySave
