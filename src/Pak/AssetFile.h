#ifndef SATISFACTORY3DMAP_ASSETUTIL_H
#define SATISFACTORY3DMAP_ASSETUTIL_H

#include <vector>

#include "GameTypes/Name.h"
#include "GameTypes/Serialization/PackageFileSummary.h"

namespace Satisfactory3DMap {
    struct NameEntrySerialized {
        std::string Name;

        void serialize(Archive& ar) {
            ar << Name;
            uint16_t DummyHashes[2]{0, 0};
            ar << DummyHashes[0];
            ar << DummyHashes[1];
        }
    };

    // FObjectImport
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Public/UObject/ObjectResource.h#L415
    struct ObjectImport {
        FName ClassPackage;     // FName
        FName ClassName;        // FName
        int32_t OuterIndex = 0; // FPackageIndex
        FName ObjectName;       // FName
    };

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
        Guid PackageGuid;
        uint32_t PackageFlags = 0;
        bool bNotAlwaysLoadedForEditorGame = false;
        bool bIsAsset = false;
        int32_t FirstExportDependency = 0;
        int32_t SerializationBeforeSerializationDependencies = 0;
        int32_t CreateBeforeSerializationDependencies = 0;
        int32_t SerializationBeforeCreateDependencies = 0;
        int32_t CreateBeforeCreateDependencies = 0;
    };

    class AssetFile {
    public:
        AssetFile(std::vector<char> uassetData, std::vector<char> uexpData);

    protected:
        // Extra struct to serialize FName index entries
        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Public/UObject/LinkerLoad.h#L854-L878
        struct NameEntry {
            int32_t NameIndex = 0;
            int32_t Number = 0;

            void serialize(Archive& ar) {
                ar << NameIndex;
                ar << Number;
            }
        };

        FName getName(const NameEntry& nameEntry);

        std::vector<char> uassetData_;
        std::vector<char> uexpData_;

        PackageFileSummary summary_;
        std::vector<NameEntrySerialized> nameMap_;
        std::vector<ObjectImport> importMap_;
        std::vector<ObjectExport> exportMap_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ASSETUTIL_H
