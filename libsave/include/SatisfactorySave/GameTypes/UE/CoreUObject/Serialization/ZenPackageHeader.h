#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../../../../IO/Archive/Archive.h"
#include "../../Core/Serialization/MappedName.h"
#include "../../Core/UObject/NameTypes.h"
#include "AsyncLoading2.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    struct SATISFACTORYSAVE_API FZenPackageHeader {
        FZenPackageSummary PackageSummary;
        std::optional<FZenPackageVersioningInfo> VersioningInfo;
        FNameMap NameMap;
        std::vector<FBulkDataMapEntry> BulkDataMap;
        std::vector<uint64_t> ImportedPublicExportHashes;
        std::vector<FPackageObjectIndex> ImportMap;
        std::vector<FExportMapEntry> ExportMap;
        std::vector<FExportBundleEntry> ExportBundleEntries;
        std::vector<FDependencyBundleHeader> DependencyBundleHeaders;
        std::vector<FDependencyBundleEntry> DependencyBundleEntries;
        std::vector<FName> ImportedPackageNames;

        void serialize(Archive& ar);
    };
} // namespace SatisfactorySave
