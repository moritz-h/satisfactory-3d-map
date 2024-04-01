#include "Pak/Serialization/PackageFileSummary.h"

void SatisfactorySave::PackageFileSummary::serialize(Archive& ar) {
    ar << Tag;
    if (Tag != 0x9E2A83C1) {
        throw std::runtime_error("uasset data has bad file package tag!");
    }
    ar << LegacyFileVersion;
    if (LegacyFileVersion != -8) {
        throw std::runtime_error("'LegacyFileVersion != -8' not implemented!");
    }
    ar << LegacyUE3Version;
    ar << FileVersionUE4;
    ar << FileVersionUE5;
    ar << FileVersionLicenseeUE;
    // CustomVersions array. Seems to be of type TArray. Assume empty array here.
    int32_t CustomVersionArrayNum = 0;
    ar << CustomVersionArrayNum;
    if (CustomVersionArrayNum != 0) {
        throw std::runtime_error("'CustomVersionArrayNum != 0' not implemented!");
    }

    ar << TotalHeaderSize;
    ar << PackageName;
    ar << PackageFlags;
    if (!(PackageFlags & PKG_FilterEditorOnly)) {
        // No support for editor data!
        throw std::runtime_error("'PackageFlag PKG_FilterEditorOnly is not set!");
    }

    ar << NameCount;
    ar << NameOffset;

    ar << SoftObjectPathsCount;
    ar << SoftObjectPathsOffset;

    ar << GatherableTextDataCount;
    ar << GatherableTextDataOffset;

    ar << ExportCount;
    ar << ExportOffset;
    ar << ImportCount;
    ar << ImportOffset;
    ar << DependsOffset;

    ar << SoftPackageReferencesCount;
    ar << SoftPackageReferencesOffset;

    ar << SearchableNamesOffset;

    ar << ThumbnailTableOffset;

    ar << Guid;

    // TArray<FGenerationInfo> Generations
    int32_t GenerationCount = static_cast<int32_t>(Generations.size());
    ar << GenerationCount;
    Generations.resize(GenerationCount);
    for (auto& g : Generations) {
        ar << g;
    }

    ar << SavedByEngineVersion;
    ar << CompatibleWithEngineVersion;

    ar << CompressionFlags;
    // TArray<FCompressedChunk> CompressedChunks
    int32_t CompressedChunksNum = 0;
    ar << CompressedChunksNum;
    if (CompressedChunksNum != 0) {
        throw std::runtime_error("Old format, not supported by Unreal itself!");
    }

    ar << PackageSource;

    // TArray<FString> AdditionalPackagesToCook;
    int32_t AdditionalPackagesToCookNum = 0;
    ar << AdditionalPackagesToCookNum;
    if (AdditionalPackagesToCookNum != 0) {
        throw std::runtime_error("'AdditionalPackagesToCookNum != 0' not implemented!");
    }

    ar << AssetRegistryDataOffset;
    ar << BulkDataStartOffset;

    ar << WorldTileInfoDataOffset;

    // TArray<int32> ChunkIDs
    ar << ChunkIDs;

    ar << PreloadDependencyCount;
    ar << PreloadDependencyOffset;

    ar << NamesReferencedFromExportDataCount;
    ar << PayloadTocOffset;
    ar << DataResourceOffset;
}
