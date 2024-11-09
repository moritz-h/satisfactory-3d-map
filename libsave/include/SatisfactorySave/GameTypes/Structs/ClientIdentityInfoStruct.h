#pragma once

#include "../FactoryGame/Online/ClientIdentification.h"
#include "Base/StructImpl.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ClientIdentityInfoStruct final
        : public StructImpl<ClientIdentityInfoStruct, FClientIdentityInfo> {
    public:
        static constexpr std::string_view TypeName = "ClientIdentityInfo";

        using StructImpl<ClientIdentityInfoStruct, FClientIdentityInfo>::StructImpl;
    };
} // namespace SatisfactorySave
