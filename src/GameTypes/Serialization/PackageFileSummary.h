#pragma once

#include <vector>

#include "GameTypes/Guid.h"
#include "IO/Archive/Archive.h"

namespace Satisfactory3DMap {

    enum EPackageFlags : uint32_t {
        // only copied used entries
        PKG_None = 0x00000000,
        PKG_FilterEditorOnly = 0x80000000,
    };

    struct FGenerationInfo {
        int32_t ExportCount = 0;
        int32_t NameCount = 0;

        void serialize(Archive& ar) {
            ar << ExportCount;
            ar << NameCount;
        }
    };

    struct EngineVersion {
        uint16_t Major = 0;
        uint16_t Minor = 0;
        uint16_t Patch = 0;
        uint32_t Changelist = 0;
        std::string Branch;

        void serialize(Archive& ar) {
            ar << Major;
            ar << Minor;
            ar << Patch;
            ar << Changelist;
            ar << Branch;
        }
    };

    // FPackageFileSummary
    struct PackageFileSummary {
        int32_t Tag = 0x9E2A83C1;
        int32_t LegacyFileVersion = 0;
        int32_t LegacyUE3Version = 0;
        int32_t FileVersionUE4 = 0;
        int32_t FileVersionLicenseeUE4 = 0;
        int32_t TotalHeaderSize = 0;
        std::string FolderName;
        uint32_t PackageFlags = 0;
        int32_t NameCount = 0;
        int32_t NameOffset = 0;
        int32_t GatherableTextDataCount = 0;
        int32_t GatherableTextDataOffset = 0;
        int32_t ExportCount = 0;
        int32_t ExportOffset = 0;
        int32_t ImportCount = 0;
        int32_t ImportOffset = 0;
        int32_t DependsOffset = 0;
        int32_t SoftPackageReferencesCount = 0;
        int32_t SoftPackageReferencesOffset = 0;
        int32_t SearchableNamesOffset = 0;
        int32_t ThumbnailTableOffset = 0;
        FGuid Guid;
        std::vector<FGenerationInfo> Generations;
        EngineVersion SavedByEngineVersion;
        EngineVersion CompatibleWithEngineVersion;
        uint32_t CompressionFlags = 0;
        uint32_t PackageSource = 0;
        int32_t AssetRegistryDataOffset = 0;
        int64_t BulkDataStartOffset = 0;
        int32_t WorldTileInfoDataOffset = 0;
        std::vector<int32_t> ChunkIDs;
        int32_t PreloadDependencyCount = 0;
        int32_t PreloadDependencyOffset = 0;

        void serialize(Archive& ar);
    };
} // namespace Satisfactory3DMap
