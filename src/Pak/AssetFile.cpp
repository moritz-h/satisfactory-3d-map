#include "AssetFile.h"

#include "GameTypes/Name.h"
#include "GameTypes/ObjectReference.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/MemoryStreams.h"

Satisfactory3DMap::AssetFile::AssetFile(const std::vector<char>& uassetData, const std::vector<char>& uexpData) {
    // Make continuous buffer, offset values in file assume this format.
    std::unique_ptr<std::vector<char>> buf = std::make_unique<std::vector<char>>(uassetData);
    buf->insert(buf->end(), uexpData.begin(), uexpData.end());
    istream_ = std::make_unique<MemIStream>(std::move(buf));

    // Parse uasset
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/LinkerLoad.cpp#L716
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/LinkerLoad.cpp#L1173
    // Header is FPackageFileSummary
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PackageFileSummary.cpp#L48

    *this << summary_;

    // Validate size of buffer
    if (summary_.TotalHeaderSize != uassetData.size()) {
        throw std::runtime_error("Unexpected asset data size!");
    }

    // End FPackageFileSummary header

    // Debug only!
    if (tell() != summary_.NameOffset) {
        throw std::runtime_error("Unknown format!");
    }

    nameMap_.resize(summary_.NameCount);
    for (auto& name : nameMap_) {
        *this << name;
    }

    // Debug only!
    if (tell() != summary_.ImportOffset) {
        throw std::runtime_error("Unknown format!");
    }

    importMap_.resize(summary_.ImportCount);
    for (auto& importEntry : importMap_) {
        *this << importEntry;
    }

    // Debug only!
    if (tell() != summary_.ExportOffset) {
        throw std::runtime_error("Unknown format!");
    }

    exportMap_.resize(summary_.ExportCount);
    for (auto& exportEntry : exportMap_) {
        *this << exportEntry;
    }

    // Debug only!
    if (tell() != summary_.DependsOffset) {
        throw std::runtime_error("Unknown format!");
    }

    // TODO remaining bytes
}

void Satisfactory3DMap::AssetFile::serializeName(Satisfactory3DMap::FName& n) {
    NameEntry nameEntry;
    *this << nameEntry;

    if (nameEntry.NameIndex < 0 || nameEntry.NameIndex >= nameMap_.size()) {
        throw std::runtime_error("nameEntry.NameIndex out of range!");
    }
    n.Name = nameMap_[nameEntry.NameIndex].Name;
    n.Number = nameEntry.Number;
}

void Satisfactory3DMap::AssetFile::serializeObjectReference(Satisfactory3DMap::ObjectReference& ref) {
    // TODO
    int32_t todo;
    *this << todo;
}
