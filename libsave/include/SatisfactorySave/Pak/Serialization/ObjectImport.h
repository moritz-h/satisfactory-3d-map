#pragma once

#include "../../GameTypes/UE/Core/UObject/NameTypes.h"
#include "../../IO/Archive/Archive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // FObjectImport
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Public/UObject/ObjectResource.h#L415
    struct SATISFACTORYSAVE_API ObjectImport {
        FName ClassPackage;
        FName ClassName;
        int32_t OuterIndex = 0; // FPackageIndex
        FName ObjectName;
        bool bImportOptional = false;

        void serialize(Archive& ar) {
            ar << ClassPackage;
            ar << ClassName;
            ar << OuterIndex;
            ar << ObjectName;
            ar << bImportOptional;
        }
    };

} // namespace SatisfactorySave
