#include "AssetUtil.h"

#include "GameTypes/Guid.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::AssetUtil::AssetUtil(const std::vector<char>& uassetData, const std::vector<char>& uexpData) {
    std::unique_ptr<std::vector<char>> buf = std::make_unique<std::vector<char>>(uassetData);
    MemIStream stream(std::move(buf));

    // Parse uasset
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/LinkerLoad.cpp#L716
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/LinkerLoad.cpp#L1173
    // Header is FPackageFileSummary
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PackageFileSummary.cpp#L48

    const int32_t Tag = read<int32_t>(stream);
    if (Tag != 0x9E2A83C1) {
        throw std::runtime_error("uasset data has bad file package tag!");
    }
    const int32_t LegacyFileVersion = read<int32_t>(stream);
    if (LegacyFileVersion != -7) {
        throw std::runtime_error("'LegacyFileVersion != -7' not implemented!");
    }
    const int32_t LegacyUE3Version = read<int32_t>(stream);
    const int32_t FileVersionUE4 = read<int32_t>(stream);
    const int32_t FileVersionLicenseeUE4 = read<int32_t>(stream);
    // CustomVersions array. Seems to be of type TArray. Assume empty array here.
    const int32_t CustomVersionArrayNum = read<int32_t>(stream);
    if (CustomVersionArrayNum != 0) {
        throw std::runtime_error("'CustomVersionArrayNum != 0' not implemented!");
    }

    const int32_t TotalHeaderSize = read<int32_t>(stream);
    const std::string FolderName = read_length_string(stream);
    const uint32_t PackageFlags = read<uint32_t>(stream);
    static const uint32_t PKG_FilterEditorOnly = 0x80000000;
    if (PackageFlags != PKG_FilterEditorOnly) {
        throw std::runtime_error("'PackageFlags != PKG_FilterEditorOnly' not implemented!");
    }

    const int32_t NameCount = read<int32_t>(stream);
    const int32_t NameOffset = read<int32_t>(stream);

    const int32_t GatherableTextDataCount = read<int32_t>(stream);
    const int32_t GatherableTextDataOffset = read<int32_t>(stream);

    const int32_t ExportCount = read<int32_t>(stream);
    const int32_t ExportOffset = read<int32_t>(stream);
    const int32_t ImportCount = read<int32_t>(stream);
    const int32_t ImportOffset = read<int32_t>(stream);
    const int32_t DependsOffset = read<int32_t>(stream);

    const int32_t SoftPackageReferencesCount = read<int32_t>(stream);
    const int32_t SoftPackageReferencesOffset = read<int32_t>(stream);

    const int32_t SearchableNamesOffset = read<int32_t>(stream);

    const int32_t ThumbnailTableOffset = read<int32_t>(stream);

    Guid guid(stream);

    const int32_t GenerationCount = read<int32_t>(stream);
    for (int32_t i = 0; i < GenerationCount; i++) {
        const int32_t ExportCount = read<int32_t>(stream);
        const int32_t NameCount = read<int32_t>(stream);
    }

    // FEngineVersion
    uint16_t SavedByEngineVersionMajor = read<uint16_t>(stream);
    uint16_t SavedByEngineVersionMinor = read<uint16_t>(stream);
    uint16_t SavedByEngineVersionPatch = read<uint16_t>(stream);
    uint32_t SavedByEngineVersionChangelist = read<uint32_t>(stream);
    std::string SavedByEngineVersionBranch = read_length_string(stream);

    uint16_t CompatibleWithEngineVersionMajor = read<uint16_t>(stream);
    uint16_t CompatibleWithEngineVersionMinor = read<uint16_t>(stream);
    uint16_t CompatibleWithEngineVersionPatch = read<uint16_t>(stream);
    uint32_t CompatibleWithEngineVersionChangelist = read<uint32_t>(stream);
    std::string CompatibleWithEngineVersionBranch = read_length_string(stream);

    uint32_t CompressionFlags = read<uint32_t>(stream);
    // TArray CompressedChunks
    int32_t CompressedChunksNum = read<int32_t>(stream);
    if (CompressedChunksNum != 0) {
        throw std::runtime_error("'CompressedChunksNum != 0' not implemented!");
    }

    uint32_t PackageSource = read<uint32_t>(stream);

    // TArray<FString> AdditionalPackagesToCook;
    int32_t AdditionalPackagesToCookNum = read<int32_t>(stream);
    if (AdditionalPackagesToCookNum != 0) {
        throw std::runtime_error("'AdditionalPackagesToCookNum != 0' not implemented!");
    }

    int32_t AssetRegistryDataOffset = read<int32_t>(stream);
    int64_t BulkDataStartOffset = read<int64_t>(stream);

    int32_t WorldTileInfoDataOffset = read<int32_t>(stream);
    // TArray<int32> ChunkIDs
    int32_t ChunkIDsNum = read<int32_t>(stream);
    if (ChunkIDsNum != 0) {
        throw std::runtime_error("'ChunkIDsNum != 0' not implemented!");
    }

    int32_t PreloadDependencyCount = read<int32_t>(stream);
    int32_t PreloadDependencyOffset = read<int32_t>(stream);

    // End FPackageFileSummary header
}
