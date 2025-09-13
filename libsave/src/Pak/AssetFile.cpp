#include "Pak/AssetFile.h"

#include <iomanip>
#include <sstream>

#include "GameTypes/FactoryGame/FGObjectReference.h"
#include "GameTypes/UE/Core/UObject/NameTypes.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/MemoryStreams.h"

SatisfactorySave::AssetFile::AssetFile(const std::vector<char>& uassetData, const std::vector<char>& ubulkData) {
    // Make continuous buffer, offset values in file assume this format.
    std::unique_ptr<std::vector<char>> buf = std::make_unique<std::vector<char>>(uassetData);
    istream_ = std::make_unique<MemIStream>(std::move(buf));
    if (!ubulkData.empty()) {
        std::unique_ptr<std::vector<char>> ubulk_buf = std::make_unique<std::vector<char>>(ubulkData);
        ubulk_ar_ = std::make_unique<IStreamArchive>(std::move(std::make_unique<MemIStream>(std::move(ubulk_buf))));
    }

    // Check old asset format
    if (this->read_ahead<int32_t>() == 0x9E2A83C1) {
        throw std::runtime_error("Old asset format!");
    }

    *this << packageHeader_;
}

void SatisfactorySave::AssetFile::serializeName(FName& n) {
    FMappedName nameEntry;
    *this << nameEntry;

    if (nameEntry.GetIndex() < 0 || nameEntry.GetIndex() >= packageHeader_.NameMap.Num()) {
        throw std::runtime_error("FMappedName.Index out of range!");
    }
    n = packageHeader_.NameMap.GetName(nameEntry);
}

void SatisfactorySave::AssetFile::serializeObjectReference(FObjectReferenceDisc& ref) {
    // TODO
    *this << ref.pak_value_;
    // TODO: The name we are reading here is probably relative, need to add package name to get absolute path name.
    ref.PathName = "[TODO:] " + std::to_string(ref.pak_value_);
    if (ref.pak_value_ > 0) {
        ref.PathName += " (ExportMap[" + std::to_string(ref.pak_value_ - 1) + "])";
    } else if (ref.pak_value_ < 0) {
        ref.PathName += " (ImportMap[" + std::to_string(-ref.pak_value_ - 1) + "])";
    }
}

bool SatisfactorySave::AssetFile::hasObjectExportEntry(const std::string& name) {
    if (!exportNameIndexMap_.has_value()) {
        exportNameIndexMap_ = std::unordered_map<std::string, std::size_t>();
        for (std::size_t i = 0; i < packageHeader_.ExportMap.size(); i++) {
            exportNameIndexMap_.value()[getNameString(packageHeader_.ExportMap[i].ObjectName)] = i;
        }
    }
    return exportNameIndexMap_.value().contains(name);
}

const SatisfactorySave::FExportMapEntry& SatisfactorySave::AssetFile::getObjectExportEntry(const std::string& name) {
    // Check is also used to trigger exportNameIndexMap_ initialization.
    if (!hasObjectExportEntry(name)) {
        throw std::runtime_error("No export entry named " + name + "!");
    }
    return packageHeader_.ExportMap[exportNameIndexMap_.value()[name]];
}

bool SatisfactorySave::AssetFile::hasObjectExportEntry(uint64_t publicExportHash) {
    if (!exportHashIndexMap_.has_value()) {
        exportHashIndexMap_ = std::unordered_map<uint64_t, std::size_t>();
        for (std::size_t i = 0; i < packageHeader_.ExportMap.size(); i++) {
            exportHashIndexMap_.value()[packageHeader_.ExportMap[i].PublicExportHash] = i;
        }
    }
    return exportHashIndexMap_.value().contains(publicExportHash);
}

const SatisfactorySave::FExportMapEntry& SatisfactorySave::AssetFile::getObjectExportEntry(uint64_t publicExportHash) {
    // Check is also used to trigger exportHashIndexMap_ initialization.
    if (!hasObjectExportEntry(publicExportHash)) {
        throw std::runtime_error("No export entry named " + std::to_string(publicExportHash) + "!");
    }
    return packageHeader_.ExportMap[exportHashIndexMap_.value()[publicExportHash]];
}
