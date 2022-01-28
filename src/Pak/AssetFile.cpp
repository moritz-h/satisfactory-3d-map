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

    ar << summary_;

    // End FPackageFileSummary header

    // Debug only!
    if (ar.tell() != summary_.NameOffset) {
        throw std::runtime_error("Unknown format!");
    }

    for (int32_t i = 0; i < summary_.NameCount; i++) {
        // FNameEntrySerialized
        std::string name;
        ar << name;
        // DummyHashes
        ar.read<uint16_t>();
        ar.read<uint16_t>();
    }

    // Debug only!
    if (ar.tell() != summary_.ImportOffset) {
        throw std::runtime_error("Unknown format!");
    }

    // For FName see: https://github.com/gildor2/UEViewer/blob/master/Unreal/UnrealPackage/UnPackage.cpp#L713

    for (int32_t i = 0; i < summary_.ImportCount; i++) {
        // FObjectImport
        ar.read<int64_t>();                      // FName ClassPackage;
        ar.read<int64_t>();                      // FName ClassName;
        int32_t OuterIndex = ar.read<int32_t>(); // FPackageIndex
        ar.read<int64_t>();                      // FName ObjectName;
    }

    // Debug only!
    if (ar.tell() != summary_.ExportOffset) {
        throw std::runtime_error("Unknown format!");
    }

    for (int32_t i = 0; i < summary_.ExportCount; i++) {
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
    if (ar.tell() != summary_.DependsOffset) {
        throw std::runtime_error("Unknown format!");
    }

    // TODO remaining bytes
}
