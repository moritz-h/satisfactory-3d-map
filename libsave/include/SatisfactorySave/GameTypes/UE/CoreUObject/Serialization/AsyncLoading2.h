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
    class SATISFACTORYSAVE_API FPackageImportReference {
    public:
        FPackageImportReference(uint32_t InImportedPackageIndex, uint32_t InImportedPublicExportHashIndex)
            : ImportedPackageIndex(InImportedPackageIndex),
              ImportedPublicExportHashIndex(InImportedPublicExportHashIndex) {}

        [[nodiscard]] uint32_t GetImportedPackageIndex() const {
            return ImportedPackageIndex;
        }

        [[nodiscard]] uint32_t GetImportedPublicExportHashIndex() const {
            return ImportedPublicExportHashIndex;
        }

    private:
        uint32_t ImportedPackageIndex;
        uint32_t ImportedPublicExportHashIndex;
    };

    class SATISFACTORYSAVE_API FPackageObjectIndex {
    private:
        static constexpr uint64_t IndexBits = 62ull;
        static constexpr uint64_t IndexMask = (1ull << IndexBits) - 1ull;
        static constexpr uint64_t TypeMask = ~IndexMask;
        static constexpr uint64_t TypeShift = IndexBits;
        static constexpr uint64_t Invalid = ~0ull;

        uint64_t TypeAndId = Invalid;

        enum EType {
            Export,
            ScriptImport,
            PackageImport,
            Null,
        };

    public:
        [[nodiscard]] inline bool IsNull() const {
            return TypeAndId == Invalid;
        }

        [[nodiscard]] inline bool IsExport() const {
            return (TypeAndId >> TypeShift) == Export;
        }

        [[nodiscard]] inline bool IsImport() const {
            return IsScriptImport() || IsPackageImport();
        }

        [[nodiscard]] inline bool IsScriptImport() const {
            return (TypeAndId >> TypeShift) == ScriptImport;
        }

        [[nodiscard]] inline bool IsPackageImport() const {
            return (TypeAndId >> TypeShift) == PackageImport;
        }

        [[nodiscard]] inline uint32_t ToExport() const {
            return static_cast<uint32_t>(TypeAndId);
        }

        [[nodiscard]] inline FPackageImportReference ToPackageImportRef() const {
            auto ImportedPackageIndex = static_cast<uint32_t>((TypeAndId & IndexMask) >> 32);
            auto ExportHash = static_cast<uint32_t>(TypeAndId);
            return FPackageImportReference(ImportedPackageIndex, ExportHash);
        }

        [[nodiscard]] inline uint64_t Value() const {
            return TypeAndId & IndexMask;
        }

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

    struct SATISFACTORYSAVE_API FScriptObjectEntry {
        FMappedName Mapped;
        FPackageObjectIndex GlobalIndex;
        FPackageObjectIndex OuterIndex;
        FPackageObjectIndex CDOClassIndex;

        void serialize(Archive& ar) {
            ar << Mapped;
            ar << GlobalIndex;
            ar << OuterIndex;
            ar << CDOClassIndex;
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
