#pragma once

#include "../UE/Core/Math/Box.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API Box3fStruct final : public StructImpl<Box3fStruct, FBox3f> {
    public:
        static constexpr std::string_view TypeName = "Box3f";

        using StructImpl<Box3fStruct, FBox3f>::StructImpl;
    };
} // namespace SatisfactorySave
