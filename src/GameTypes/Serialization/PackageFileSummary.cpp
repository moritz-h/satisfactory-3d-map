#include "PackageFileSummary.h"

void Satisfactory3DMap::PackageFileSummary::serialize(Archive& ar) {
    ar << Tag;
    if (Tag != 0x9E2A83C1) {
        throw std::runtime_error("uasset data has bad file package tag!");
    }
    ar << LegacyFileVersion;
    if (LegacyFileVersion != -7) {
        throw std::runtime_error("'LegacyFileVersion != -7' not implemented!");
    }
    ar << LegacyUE3Version;
    ar << FileVersionUE4;
    ar << FileVersionLicenseeUE4;
    // CustomVersions array. Seems to be of type TArray. Assume empty array here.
    int32_t CustomVersionArrayNum = 0;
    ar << CustomVersionArrayNum;
    if (CustomVersionArrayNum != 0) {
        throw std::runtime_error("'CustomVersionArrayNum != 0' not implemented!");
    }

    ar << TotalHeaderSize;
    ar << FolderName;
    ar << PackageFlags;
    static constexpr uint32_t PKG_FilterEditorOnly = 0x80000000;
    if (PackageFlags != PKG_FilterEditorOnly) {
        throw std::runtime_error("'PackageFlags != PKG_FilterEditorOnly' not implemented!");
    }

    ar << NameCount;
    ar << NameOffset;

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
    int32_t ChunkIDsNum = static_cast<int32_t>(ChunkIDs.size());
    ar << ChunkIDsNum;
    ChunkIDs.resize(ChunkIDsNum);
    for (auto& c : ChunkIDs) {
        ar << c;
    }

    ar << PreloadDependencyCount;
    ar << PreloadDependencyOffset;
}
