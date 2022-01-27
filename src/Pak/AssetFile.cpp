#include "AssetFile.h"

#include "GameTypes/Guid.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/MemoryStreams.h"

Satisfactory3DMap::AssetFile::AssetFile(std::vector<char> uassetData, std::vector<char> uexpData)
    : uassetData_(std::move(uassetData)),
      uexpData_(std::move(uexpData)) {
    std::unique_ptr<std::vector<char>> buf = std::make_unique<std::vector<char>>(uassetData_);
    IStreamArchive ar(std::make_unique<MemIStream>(std::move(buf)));

    // Parse uasset
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/LinkerLoad.cpp#L716
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/LinkerLoad.cpp#L1173
    // Header is FPackageFileSummary
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PackageFileSummary.cpp#L48

    const int32_t Tag = ar.read<int32_t>();
    if (Tag != 0x9E2A83C1) {
        throw std::runtime_error("uasset data has bad file package tag!");
    }
    const int32_t LegacyFileVersion = ar.read<int32_t>();
    if (LegacyFileVersion != -7) {
        throw std::runtime_error("'LegacyFileVersion != -7' not implemented!");
    }
    const int32_t LegacyUE3Version = ar.read<int32_t>();
    const int32_t FileVersionUE4 = ar.read<int32_t>();
    const int32_t FileVersionLicenseeUE4 = ar.read<int32_t>();
    // CustomVersions array. Seems to be of type TArray. Assume empty array here.
    const int32_t CustomVersionArrayNum = ar.read<int32_t>();
    if (CustomVersionArrayNum != 0) {
        throw std::runtime_error("'CustomVersionArrayNum != 0' not implemented!");
    }

    const int32_t TotalHeaderSize = ar.read<int32_t>();
    std::string FolderName;
    ar << FolderName;
    const uint32_t PackageFlags = ar.read<uint32_t>();
    static const uint32_t PKG_FilterEditorOnly = 0x80000000;
    if (PackageFlags != PKG_FilterEditorOnly) {
        throw std::runtime_error("'PackageFlags != PKG_FilterEditorOnly' not implemented!");
    }

    const int32_t NameCount = ar.read<int32_t>();
    const int32_t NameOffset = ar.read<int32_t>();

    const int32_t GatherableTextDataCount = ar.read<int32_t>();
    const int32_t GatherableTextDataOffset = ar.read<int32_t>();

    const int32_t ExportCount = ar.read<int32_t>();
    const int32_t ExportOffset = ar.read<int32_t>();
    const int32_t ImportCount = ar.read<int32_t>();
    const int32_t ImportOffset = ar.read<int32_t>();
    const int32_t DependsOffset = ar.read<int32_t>();

    const int32_t SoftPackageReferencesCount = ar.read<int32_t>();
    const int32_t SoftPackageReferencesOffset = ar.read<int32_t>();

    const int32_t SearchableNamesOffset = ar.read<int32_t>();

    const int32_t ThumbnailTableOffset = ar.read<int32_t>();

    Guid guid;
    ar << guid;

    const int32_t GenerationCount = ar.read<int32_t>();
    for (int32_t i = 0; i < GenerationCount; i++) {
        const int32_t ExportCount = ar.read<int32_t>();
        const int32_t NameCount = ar.read<int32_t>();
    }

    // FEngineVersion
    uint16_t SavedByEngineVersionMajor = ar.read<uint16_t>();
    uint16_t SavedByEngineVersionMinor = ar.read<uint16_t>();
    uint16_t SavedByEngineVersionPatch = ar.read<uint16_t>();
    uint32_t SavedByEngineVersionChangelist = ar.read<uint32_t>();
    std::string SavedByEngineVersionBranch;
    ar << SavedByEngineVersionBranch;

    uint16_t CompatibleWithEngineVersionMajor = ar.read<uint16_t>();
    uint16_t CompatibleWithEngineVersionMinor = ar.read<uint16_t>();
    uint16_t CompatibleWithEngineVersionPatch = ar.read<uint16_t>();
    uint32_t CompatibleWithEngineVersionChangelist = ar.read<uint32_t>();
    std::string CompatibleWithEngineVersionBranch;
    ar << CompatibleWithEngineVersionBranch;

    uint32_t CompressionFlags = ar.read<uint32_t>();
    // TArray CompressedChunks
    int32_t CompressedChunksNum = ar.read<int32_t>();
    if (CompressedChunksNum != 0) {
        throw std::runtime_error("'CompressedChunksNum != 0' not implemented!");
    }

    uint32_t PackageSource = ar.read<uint32_t>();

    // TArray<FString> AdditionalPackagesToCook;
    int32_t AdditionalPackagesToCookNum = ar.read<int32_t>();
    if (AdditionalPackagesToCookNum != 0) {
        throw std::runtime_error("'AdditionalPackagesToCookNum != 0' not implemented!");
    }

    int32_t AssetRegistryDataOffset = ar.read<int32_t>();
    int64_t BulkDataStartOffset = ar.read<int64_t>();

    int32_t WorldTileInfoDataOffset = ar.read<int32_t>();
    // TArray<int32> ChunkIDs
    int32_t ChunkIDsNum = ar.read<int32_t>();
    if (ChunkIDsNum != 0) {
        throw std::runtime_error("'ChunkIDsNum != 0' not implemented!");
    }

    int32_t PreloadDependencyCount = ar.read<int32_t>();
    int32_t PreloadDependencyOffset = ar.read<int32_t>();

    // End FPackageFileSummary header

    // Debug only!
    if (ar.tell() != NameOffset) {
        throw std::runtime_error("Unknown format!");
    }

    for (int32_t i = 0; i < NameCount; i++) {
        // FNameEntrySerialized
        std::string name;
        ar << name;
        // DummyHashes
        ar.read<uint16_t>();
        ar.read<uint16_t>();
    }

    // Debug only!
    if (ar.tell() != ImportOffset) {
        throw std::runtime_error("Unknown format!");
    }

    // For FName see: https://github.com/gildor2/UEViewer/blob/master/Unreal/UnrealPackage/UnPackage.cpp#L713

    for (int32_t i = 0; i < ImportCount; i++) {
        // FObjectImport
        ar.read<int64_t>();                      // FName ClassPackage;
        ar.read<int64_t>();                      // FName ClassName;
        int32_t OuterIndex = ar.read<int32_t>(); // FPackageIndex
        ar.read<int64_t>();                      // FName ObjectName;
    }

    // Debug only!
    if (ar.tell() != ExportOffset) {
        throw std::runtime_error("Unknown format!");
    }

    for (int32_t i = 0; i < ExportCount; i++) {
        // FObjectExport
        ar.read<int32_t>(); // FPackageIndex ClassIndex;
        ar.read<int32_t>(); // FPackageIndex SuperIndex;
        ar.read<int32_t>(); // FPackageIndex TemplateIndex;
        ar.read<int32_t>(); // FPackageIndex OuterIndex;
        ar.read<int64_t>(); // FName ObjectName;
        uint32_t Save = ar.read<uint32_t>();
        int64_t SerialSize = ar.read<int64_t>();
        int64_t SerialOffset = ar.read<int64_t>();
        bool bForcedExport = !!ar.read<uint32_t>();
        bool bNotForClient = !!ar.read<uint32_t>();
        bool bNotForServer = !!ar.read<uint32_t>();
        Guid PackageGuid;
        ar << PackageGuid;
        uint32_t PackageFlags = ar.read<uint32_t>();
        bool bNotAlwaysLoadedForEditorGame = !!ar.read<uint32_t>();
        bool bIsAsset = !!ar.read<uint32_t>();
        int32_t FirstExportDependency = ar.read<int32_t>();
        int32_t SerializationBeforeSerializationDependencies = ar.read<int32_t>();
        int32_t CreateBeforeSerializationDependencies = ar.read<int32_t>();
        int32_t SerializationBeforeCreateDependencies = ar.read<int32_t>();
        int32_t CreateBeforeCreateDependencies = ar.read<int32_t>();
    }

    // Debug only!
    if (ar.tell() != DependsOffset) {
        throw std::runtime_error("Unknown format!");
    }

    // TODO remaining bytes
}
