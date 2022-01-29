#ifndef SATISFACTORY3DMAP_OBJECTIMPORT_H
#define SATISFACTORY3DMAP_OBJECTIMPORT_H

#include "../Name.h"
#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    // FObjectImport
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Public/UObject/ObjectResource.h#L415
    struct ObjectImport {
        FName ClassPackage;     // FName
        FName ClassName;        // FName
        int32_t OuterIndex = 0; // FPackageIndex
        FName ObjectName;       // FName

        void serialize(Archive& ar) {
            ar << ClassPackage;
            ar << ClassName;
            ar << OuterIndex;
            ar << ObjectName;
        }
    };

} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_OBJECTIMPORT_H
