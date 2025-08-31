#include "GameTypes/UE/CoreUObject/Serialization/ZenPackageHeader.h"

#include "GameTypes/UE/Core/UObject/NameBatchSerialization.h"
#include "Utils/VectorUtils.h"

void SatisfactorySave::FZenPackageHeader::serialize(Archive& ar) {
    if (!ar.isIArchive()) {
        throw std::runtime_error("Only IStreamArchive support implemented!");
    }

    // https://github.com/EpicGames/UnrealEngine/blob/5.3.2-release/Engine/Source/Runtime/CoreUObject/Private/Serialization/ZenPackageHeader.cpp#L7

    ar << PackageSummary;

    if (PackageSummary.bHasVersioningInfo) {
        VersioningInfo.emplace();
        ar << VersioningInfo.value();
    }

    ar << NameMap;

    int64_t BulkDataMapSize = 0;
    ar << BulkDataMapSize;

    BulkDataMap.resize(BulkDataMapSize / sizeof(FBulkDataMapEntry));
    ar.serializeRaw(BulkDataMap.data(), vector_bin_size(BulkDataMap));

    // Debug only!
    if (ar.tell() != PackageSummary.ImportedPublicExportHashesOffset) {
        throw std::runtime_error("Unexpected ImportedPublicExportHashesOffset!");
    }

    ImportedPublicExportHashes.resize(
        (PackageSummary.ImportMapOffset - PackageSummary.ImportedPublicExportHashesOffset) / sizeof(uint64_t));
    ar.serializeRaw(ImportedPublicExportHashes.data(), vector_bin_size(ImportedPublicExportHashes));

    ImportMap.resize((PackageSummary.ExportMapOffset - PackageSummary.ImportMapOffset) / sizeof(FPackageObjectIndex));
    ar.serializeRaw(ImportMap.data(), vector_bin_size(ImportMap));

    ExportMap.resize(
        (PackageSummary.ExportBundleEntriesOffset - PackageSummary.ExportMapOffset) / sizeof(FExportMapEntry));
    ar.serializeRaw(ExportMap.data(), vector_bin_size(ExportMap));

    ExportBundleEntries.resize(
        (PackageSummary.DependencyBundleHeadersOffset - PackageSummary.ExportBundleEntriesOffset) /
        sizeof(FExportBundleEntry));
    ar.serializeRaw(ExportBundleEntries.data(), vector_bin_size(ExportBundleEntries));

    DependencyBundleHeaders.resize(
        (PackageSummary.DependencyBundleEntriesOffset - PackageSummary.DependencyBundleHeadersOffset) /
        sizeof(FDependencyBundleHeader));
    ar.serializeRaw(DependencyBundleHeaders.data(), vector_bin_size(DependencyBundleHeaders));

    DependencyBundleEntries.resize(
        (PackageSummary.ImportedPackageNamesOffset - PackageSummary.DependencyBundleEntriesOffset) /
        sizeof(FDependencyBundleEntry));
    ar.serializeRaw(DependencyBundleEntries.data(), vector_bin_size(DependencyBundleEntries));

    const auto NameEntries = LoadNameBatch(ar);
    ImportedPackageNames.reserve(NameEntries.size());
    for (const auto& name : NameEntries) {
        int32_t Number = 0;
        ar << Number;
        ImportedPackageNames.emplace_back(name, Number);
    }

    // Debug only!
    if (ar.tell() != PackageSummary.HeaderSize) {
        throw std::runtime_error("Unexpected PackageSummary.HeaderSize!");
    }
}
