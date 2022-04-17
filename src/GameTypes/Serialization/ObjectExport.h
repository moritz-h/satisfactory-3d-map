#pragma once

#include "../Guid.h"
#include "../Name.h"
#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    // FObjectExport
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Public/UObject/ObjectResource.h#L202
    struct ObjectExport {
        int32_t ClassIndex = 0;    // FPackageIndex
        int32_t SuperIndex = 0;    // FPackageIndex
        int32_t TemplateIndex = 0; // FPackageIndex
        int32_t OuterIndex = 0;    // FPackageIndex
        FName ObjectName;          // FName
        uint32_t Save = 0;
        int64_t SerialSize = 0;
        int64_t SerialOffset = 0;
        bool bForcedExport = false;
        bool bNotForClient = false;
        bool bNotForServer = false;
        FGuid PackageGuid;
        uint32_t PackageFlags = 0;
        bool bNotAlwaysLoadedForEditorGame = false;
        bool bIsAsset = false;
        int32_t FirstExportDependency = 0;
        int32_t SerializationBeforeSerializationDependencies = 0;
        int32_t CreateBeforeSerializationDependencies = 0;
        int32_t SerializationBeforeCreateDependencies = 0;
        int32_t CreateBeforeCreateDependencies = 0;

        void serialize(Archive& ar) {
            ar << ClassIndex;
            ar << SuperIndex;
            ar << TemplateIndex;
            ar << OuterIndex;
            ar << ObjectName;
            ar << Save;
            ar << SerialSize;
            ar << SerialOffset;
            ar << bForcedExport;
            ar << bNotForClient;
            ar << bNotForServer;
            ar << PackageGuid;
            ar << PackageFlags;
            ar << bNotAlwaysLoadedForEditorGame;
            ar << bIsAsset;
            ar << FirstExportDependency;
            ar << SerializationBeforeSerializationDependencies;
            ar << CreateBeforeSerializationDependencies;
            ar << SerializationBeforeCreateDependencies;
            ar << CreateBeforeCreateDependencies;
        }
    };

} // namespace Satisfactory3DMap
