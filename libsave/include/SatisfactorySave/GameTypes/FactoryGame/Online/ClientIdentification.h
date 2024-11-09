#pragma once

#include "../../../IO/Archive/Archive.h"
#include "../../UE/Core/Containers/Map.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FClientIdentityInfo {
    public:
        std::string OfflineId;
        TMap</*UE::Online::EOnlineServices*/ uint8_t, std::vector<uint8_t>> AccountIds;
        // https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/CoreOnline/Public/Online/CoreOnline.h#L226

        void serialize(Archive& ar) {
            ar << OfflineId;
            ar << AccountIds;
        }
    };
} // namespace SatisfactorySave
