#pragma once

#include <array>
#include <cstdint>

#include "../../../../IO/Archive/Archive.h"
#include "../../Core/Serialization/CustomVersion.h"
#include "../../Core/Serialization/MappedName.h"
#include "../../Core/UObject/ObjectVersion.h"
#include "../UObject/ObjectResource.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class SATISFACTORYSAVE_API FPackageObjectIndex {
    public:
        static constexpr uint64_t Invalid = ~0ull;

        uint64_t TypeAndId = Invalid;

        void serialize(Archive& ar) {
            ar << TypeAndId;
        }
    };

    struct SATISFACTORYSAVE_API FZenPackageVersioningInfo {
        /*EZenPackageVersion*/ uint32_t ZenVersion = 0;
        FPackageFileVersion PackageVersion;
        int32_t LicenseeVersion = 0;
        FCustomVersionContainer CustomVersions;

        void serialize(Archive& ar) {
            ar << ZenVersion;
            ar << PackageVersion;
            ar << LicenseeVersion;
            ar << CustomVersions;
        }
    };

    struct SATISFACTORYSAVE_API FZenPackageSummary {
        uint32_t bHasVersioningInfo = 0;
        uint32_t HeaderSize = 0;
        FMappedName Name;
        uint32_t PackageFlags = 0;
        uint32_t CookedHeaderSize = 0;
        int32_t ImportedPublicExportHashesOffset = 0;
        int32_t ImportMapOffset = 0;
        int32_t ExportMapOffset = 0;
        int32_t ExportBundleEntriesOffset = 0;
        int32_t DependencyBundleHeadersOffset = 0;
        int32_t DependencyBundleEntriesOffset = 0;
        int32_t ImportedPackageNamesOffset = 0;

        void serialize(Archive& ar) {
            ar << bHasVersioningInfo;
            ar << HeaderSize;
            ar << Name;
            ar << PackageFlags;
            ar << CookedHeaderSize;
            ar << ImportedPublicExportHashesOffset;
            ar << ImportMapOffset;
            ar << ExportMapOffset;
            ar << ExportBundleEntriesOffset;
            ar << DependencyBundleHeadersOffset;
            ar << DependencyBundleEntriesOffset;
            ar << ImportedPackageNamesOffset;
        }
    };

    struct SATISFACTORYSAVE_API FExportBundleEntry {
        uint32_t LocalExportIndex;
        uint32_t CommandType;

        void serialize(Archive& ar) {
            ar << LocalExportIndex;
            ar << CommandType;
        }
    };

    struct SATISFACTORYSAVE_API FDependencyBundleEntry {
        FPackageIndex LocalImportOrExportIndex;

        void serialize(Archive& ar) {
            ar << LocalImportOrExportIndex;
        }
    };

    struct SATISFACTORYSAVE_API FDependencyBundleHeader {
        int32_t FirstEntryIndex = 0;
        std::array<std::array<uint32_t, 2>, 2> EntryCount{};

        void serialize(Archive& ar) {
            ar << FirstEntryIndex;
            ar << EntryCount;
        }
    };

    struct SATISFACTORYSAVE_API FExportMapEntry {
        uint64_t CookedSerialOffset = 0;
        uint64_t CookedSerialSize = 0;
        FMappedName ObjectName;
        FPackageObjectIndex OuterIndex;
        FPackageObjectIndex ClassIndex;
        FPackageObjectIndex SuperIndex;
        FPackageObjectIndex TemplateIndex;
        uint64_t PublicExportHash = 0;
        /*EObjectFlags*/ uint32_t ObjectFlags = 0;
        /*EExportFilterFlags*/ uint8_t FilterFlags = 0;
        std::array<uint8_t, 3> Pad = {};

        void serialize(Archive& ar) {
            ar << CookedSerialOffset;
            ar << CookedSerialSize;
            ar << ObjectName;
            ar << OuterIndex;
            ar << ClassIndex;
            ar << SuperIndex;
            ar << TemplateIndex;
            ar << PublicExportHash;
            ar << ObjectFlags;
            ar << FilterFlags;
            ar << Pad;
        }
    };

    struct SATISFACTORYSAVE_API FBulkDataMapEntry {
        int64_t SerialOffset = 0;
        int64_t DuplicateSerialOffset = 0;
        int64_t SerialSize = 0;
        uint32_t Flags = 0;
        uint32_t Pad = 0;

        void serialize(Archive& ar) {
            ar << SerialOffset;
            ar << DuplicateSerialOffset;
            ar << SerialSize;
            ar << Flags;
            ar << Pad;
        }
    };
} // namespace SatisfactorySave
