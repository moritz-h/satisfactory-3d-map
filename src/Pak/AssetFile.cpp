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

    nameMap_.resize(summary_.NameCount);
    for (auto& name : nameMap_) {
        ar << name;
    }

    // Debug only!
    if (ar.tell() != summary_.ImportOffset) {
        throw std::runtime_error("Unknown format!");
    }

    importMap_.resize(summary_.ImportCount);
    for (auto& importEntry : importMap_) {
        NameEntry nameEntry;
        ar << nameEntry;
        importEntry.ClassPackage = getName(nameEntry);
        ar << nameEntry;
        importEntry.ClassName = getName(nameEntry);
        ar << importEntry.OuterIndex;
        ar << nameEntry;
        importEntry.ObjectName = getName(nameEntry);
    }

    // Debug only!
    if (ar.tell() != summary_.ExportOffset) {
        throw std::runtime_error("Unknown format!");
    }

    exportMap_.resize(summary_.ExportCount);
    for (auto& exportEntry : exportMap_) {
        ar << exportEntry.ClassIndex;
        ar << exportEntry.SuperIndex;
        ar << exportEntry.TemplateIndex;
        ar << exportEntry.OuterIndex;
        NameEntry nameEntry;
        ar << nameEntry;
        exportEntry.ObjectName = getName(nameEntry);
        ar << exportEntry.Save;
        ar << exportEntry.SerialSize;
        ar << exportEntry.SerialOffset;
        ar << exportEntry.bForcedExport;
        ar << exportEntry.bNotForClient;
        ar << exportEntry.bNotForServer;
        ar << exportEntry.PackageGuid;
        ar << exportEntry.PackageFlags;
        ar << exportEntry.bNotAlwaysLoadedForEditorGame;
        ar << exportEntry.bIsAsset;
        ar << exportEntry.FirstExportDependency;
        ar << exportEntry.SerializationBeforeSerializationDependencies;
        ar << exportEntry.CreateBeforeSerializationDependencies;
        ar << exportEntry.SerializationBeforeCreateDependencies;
        ar << exportEntry.CreateBeforeCreateDependencies;
    }

    // Debug only!
    if (ar.tell() != summary_.DependsOffset) {
        throw std::runtime_error("Unknown format!");
    }

    // TODO remaining bytes
}

Satisfactory3DMap::FName Satisfactory3DMap::AssetFile::getName(
    const Satisfactory3DMap::AssetFile::NameEntry& nameEntry) {
    if (nameEntry.NameIndex < 0 || nameEntry.NameIndex >= nameMap_.size()) {
        throw std::runtime_error("nameEntry.NameIndex out of range!");
    }
    FName name;
    name.Name = nameMap_[nameEntry.NameIndex].Name;
    name.Number = nameEntry.Number;

    return name;
}
