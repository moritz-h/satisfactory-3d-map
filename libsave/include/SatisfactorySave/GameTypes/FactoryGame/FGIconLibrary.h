#pragma once

#include <cstdint>

#include "../UE/CoreUObject/UObject/TopLevelAssetPath.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API FPersistentGlobalIconId {
    public:
        FTopLevelAssetPath IconLibrary;
        int32_t IconID = 0;
    };

} // namespace SatisfactorySave
