#pragma once

#include "../../GameTypes/Misc/Name.h"
#include "../../IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    // FObjectImport
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Public/UObject/ObjectResource.h#L415
    struct ObjectImport {
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

} // namespace Satisfactory3DMap
