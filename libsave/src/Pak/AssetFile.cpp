#include "Pak/AssetFile.h"

#include <spdlog/spdlog.h>

#include "GameTypes/FactoryGame/FGObjectReference.h"
#include "GameTypes/UE/Core/UObject/NameTypes.h"
#include "IO/Archive/IStreamArchive.h"
#include "Pak/PakManager.h"

SatisfactorySave::AssetFile::AssetFile(std::shared_ptr<PakManager> pakManager, std::unique_ptr<IStream> uassetStream,
    std::unique_ptr<IStream> ubulkStream)
    : IStreamArchive(std::move(uassetStream)),
      pakManager_(std::move(pakManager)) {
    if (ubulkStream != nullptr) {
        ubulk_ar_ = std::make_unique<IStreamArchive>(std::move(ubulkStream));
    }

    // Check old asset format
    if (this->read_ahead<int32_t>() == 0x9E2A83C1) {
        throw std::runtime_error("Old asset format!");
    }

    *this << packageHeader_;

    // Fill export maps
    for (std::size_t i = 0; i < packageHeader_.ExportMap.size(); i++) {
        const auto name = getNameString(packageHeader_.ExportMap[i].ObjectName);
        exportNameIndicesMap_[name].push_back(i);

        const auto hash = packageHeader_.ExportMap[i].PublicExportHash;
        if (hash != 0) {
            if (!exportHashIndexMap_.contains(hash)) {
                exportHashIndexMap_[hash] = i;
            } else {
                spdlog::error("Duplicated export hash in asset file: {}", hash);
            }
        }
    }
}

const std::vector<std::size_t>& SatisfactorySave::AssetFile::getExportMapIndicesByName(const std::string& name) const {
    const auto it = exportNameIndicesMap_.find(name);
    if (it != exportNameIndicesMap_.end()) {
        return it->second;
    }
    static const std::vector<std::size_t> empty;
    return empty;
}

std::optional<std::size_t> SatisfactorySave::AssetFile::getExportMapIndexByHash(uint64_t publicExportHash) const {
    const auto it = exportHashIndexMap_.find(publicExportHash);
    if (it != exportHashIndexMap_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::shared_ptr<SatisfactorySave::AssetExport> SatisfactorySave::AssetFile::getExportObjectByIdx(std::size_t idx) {
    if (idx > packageHeader_.ExportMap.size()) {
        return nullptr;
    }
    const auto& exportEntry = packageHeader_.ExportMap[idx];

    auto exp = std::make_shared<AssetExport>();
    const auto class_name = pakManager_->tryGetScriptObjectFullName(exportEntry.ClassIndex);
    exp->Object = AssetUObjectFactory(class_name);

    seekCookedSerialOffset(exportEntry.CookedSerialOffset);
    auto read_limit_stack_pusher = pushReadLimit(exportEntry.CookedSerialSize);
    try {
        *this << *exp->Object;
    } catch (const std::exception& ex1) {
        // Try a second time as UObject only.
        spdlog::error("Error parsing '{}': {} (try again as UObject only!)", class_name, ex1.what());
        try {
            exp->Object = std::make_shared<UObject>();
            seekCookedSerialOffset(exportEntry.CookedSerialOffset);
            *this << *exp->Object;
        } catch (const std::exception& ex2) {
            spdlog::error("Error parsing '{}' as UObject: {}", class_name, ex2.what());
            exp->Object = nullptr;
            seekCookedSerialOffset(exportEntry.CookedSerialOffset);
        }
    }

    // Read unknown class data as binary buffer
    const auto data_read_size = tellCookedSerialOffset() - exportEntry.CookedSerialOffset;
    if (data_read_size != exportEntry.CookedSerialSize) {
        exp->BinaryClassData = read_buffer(exportEntry.CookedSerialSize - data_read_size);
    }

    return exp;
}

std::shared_ptr<SatisfactorySave::AssetExport> SatisfactorySave::AssetFile::getExportObjectByName(
    const std::string& name) {
    const auto indices = getExportMapIndicesByName(name);
    if (indices.size() == 1) {
        return getExportObjectByIdx(indices[0]);
    }
    if (indices.size() > 1) {
        spdlog::warn("Try to read non unique asset export name: {}", name);
    }
    return nullptr;
}

std::shared_ptr<SatisfactorySave::AssetExport> SatisfactorySave::AssetFile::getExportObjectByHash(
    uint64_t publicExportHash) {
    const auto idx = getExportMapIndexByHash(publicExportHash);
    if (idx.has_value()) {
        return getExportObjectByIdx(idx.value());
    }
    return nullptr;
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
