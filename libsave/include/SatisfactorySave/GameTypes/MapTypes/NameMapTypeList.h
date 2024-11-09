#pragma once

#include "../UE/Core/UObject/NameTypes.h"
#include "Base/MapTypeListImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API NameMapTypeList final : public MapTypeListImpl<NameMapTypeList, FName> {
    public:
        static constexpr std::string_view TypeName = "NameProperty";
    };
} // namespace SatisfactorySave
