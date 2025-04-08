#pragma once

#include "../UE/Core/Misc/DateTime.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API DateTimeStruct final : public StructImpl<DateTimeStruct, FDateTime> {
    public:
        static constexpr std::string_view TypeName = "DateTime";

        using StructImpl<DateTimeStruct, FDateTime>::StructImpl;
    };
} // namespace SatisfactorySave
