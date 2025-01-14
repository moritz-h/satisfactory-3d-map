#include "Pak/AssetFile.h"

#include <iomanip>
#include <sstream>

#include "GameTypes/FactoryGame/FGObjectReference.h"
#include "GameTypes/UE/Core/UObject/NameTypes.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/MemoryStreams.h"

SatisfactorySave::AssetFile::AssetFile(const std::vector<char>& uassetData, const std::vector<char>& uexpData,
    const std::vector<char>& ubulkData) {
    // Make continuous buffer, offset values in file assume this format.
    std::unique_ptr<std::vector<char>> buf = std::make_unique<std::vector<char>>(uassetData);
    buf->insert(buf->end(), uexpData.begin(), uexpData.end());
    if (!ubulkData.empty()) {
        // TODO: The offset values in the summary could probably mean, that ubulk data is appended behind the uexp data,
        //       but somehow the last 4 bytes of uexp are not counted. What does this mean?
        buf->resize(buf->size() - 4);
        buf->insert(buf->end(), ubulkData.begin(), ubulkData.end());
    }
    istream_ = std::make_unique<MemIStream>(std::move(buf));

    // Parse uasset
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/LinkerLoad.cpp#L716
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/UObject/LinkerLoad.cpp#L1173
    // Header is FPackageFileSummary
    // https://github.com/EpicGames/UnrealEngine/blob/5.2.1-release/Engine/Source/Runtime/CoreUObject/Private/UObject/PackageFileSummary.cpp#L49

    *this << summary_;

    // Validate size of buffer
    if (summary_.TotalHeaderSize != uassetData.size()) {
        throw std::runtime_error("Unexpected asset data size!");
    }

    // End FPackageFileSummary header

    // Debug only!
    if (tell() != summary_.NameOffset) {
        throw std::runtime_error("Unknown format (NameOffset)!");
    }

    nameMap_.resize(summary_.NameCount);
    for (auto& name : nameMap_) {
        *this << name;
    }

    // Debug only!
    if (tell() != summary_.SoftObjectPathsOffset) {
        throw std::runtime_error("Unknown format (SoftObjectPathsOffset)!");
    }

    // TODO
    if (summary_.SoftObjectPathsCount != 0) {
        throw std::runtime_error("SoftObjectPathsCount != 0 not implemented!");
    }

    // Debug only!
    if (tell() != summary_.ImportOffset) {
        throw std::runtime_error("Unknown format (ImportOffset)!");
    }

    importMap_.resize(summary_.ImportCount);
    for (auto& importEntry : importMap_) {
        *this << importEntry;
    }

    // Debug only!
    if (tell() != summary_.ExportOffset) {
        throw std::runtime_error("Unknown format (ExportOffset)!");
    }

    exportMap_.resize(summary_.ExportCount);
    for (auto& exportEntry : exportMap_) {
        *this << exportEntry;
    }

    // Debug only!
    if (tell() != summary_.DependsOffset) {
        throw std::runtime_error("Unknown format (DependsOffset)!");
    }

    // TODO remaining bytes

    if (summary_.DataResourceOffset > 0) {
        seek(summary_.DataResourceOffset);

        uint32_t Version = 0;
        *this << Version;
        *this << DataResourceMap;
    }
}

std::string SatisfactorySave::AssetFile::nameMapToString() const {
    std::stringstream result;
    int i = 0;
    for (const auto& name : nameMap_) {
        result << std::hex << std::setfill('0') << std::setw(2) << i << " " << name.Name << std::endl;
        i++;
    }
    return result.str();
}

void SatisfactorySave::AssetFile::serializeName(FName& n) {
    NameEntry nameEntry;
    *this << nameEntry;

    if (nameEntry.NameIndex < 0 || nameEntry.NameIndex >= nameMap_.size()) {
        throw std::runtime_error("nameEntry.NameIndex out of range!");
    }
    n = FName(nameMap_[nameEntry.NameIndex].Name, nameEntry.Number);
}

void SatisfactorySave::AssetFile::serializeObjectReference(FObjectReferenceDisc& ref) {
    // TODO
    *this << ref.pak_value_;
    // TODO: The name we are reading here is probably relative, need to add package name to get absolute path name.
    if (ref.pak_value_ > 0) {
        ref.PathName = "[TODO:]" + exportMap_[ref.pak_value_ - 1].ObjectName.toString();
    } else if (ref.pak_value_ < 0) {
        ref.PathName = "[TODO:]" + importMap_[-ref.pak_value_ - 1].ObjectName.toString();
    }
}

bool SatisfactorySave::AssetFile::hasObjectExportEntry(const std::string& name) {
    if (!exportNameIndexMap_.has_value()) {
        exportNameIndexMap_ = std::unordered_map<std::string, std::size_t>();
        for (std::size_t i = 0; i < exportMap_.size(); i++) {
            exportNameIndexMap_.value()[exportMap_[i].ObjectName.toString()] = i;
        }
    }
    return exportNameIndexMap_.value().contains(name);
}

const SatisfactorySave::ObjectExport& SatisfactorySave::AssetFile::getObjectExportEntry(const std::string& name) {
    // Check is also used to trigger exportNameIndexMap_ initialization.
    if (!hasObjectExportEntry(name)) {
        throw std::runtime_error("No export entry named " + name + "!");
    }
    return exportMap_[exportNameIndexMap_.value()[name]];
}
