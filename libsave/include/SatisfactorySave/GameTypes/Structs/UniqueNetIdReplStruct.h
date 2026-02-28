#pragma once

#include "../UE/Engine/GameFramework/OnlineReplStructs.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API UniqueNetIdReplStruct final
        : public StructImpl<UniqueNetIdReplStruct, FUniqueNetIdRepl> {
    public:
        static constexpr std::string_view TypeName = "UniqueNetIdRepl";

        using StructImpl<UniqueNetIdReplStruct, FUniqueNetIdRepl>::StructImpl;
    };
} // namespace SatisfactorySave
