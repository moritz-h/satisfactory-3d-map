#pragma once

#include "../FactoryGame/Online/PlayerInfoCache.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API PlayerInfoHandleStruct final
        : public StructImpl<PlayerInfoHandleStruct, FPlayerInfoHandle> {
    public:
        static constexpr std::string_view TypeName = "PlayerInfoHandle";

        using StructImpl<PlayerInfoHandleStruct, FPlayerInfoHandle>::StructImpl;
    };
} // namespace SatisfactorySave
