#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../GameTypes/UE/Core/Serialization/MappedName.h"
#include "../GameTypes/UE/CoreUObject/Serialization/AsyncLoading2.h"
#include "../GameTypes/UE/CoreUObject/Serialization/ZenPackageHeader.h"
#include "../IO/Archive/IStreamArchive.h"
#include "../IO/IOStream.h"
#include "AssetExport.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {
    class PakManager;

    class SATISFACTORYSAVE_API AssetFile : public IStreamArchive {
    public:
        AssetFile(std::shared_ptr<PakManager> pakManager, std::unique_ptr<IStream> uassetStream,
            std::unique_ptr<IStream> ubulkStream = nullptr);

        inline void seekCookedSerialOffset(uint64_t offset) {
            seek(packageHeader_.PackageSummary.HeaderSize + offset);
        }

        inline uint64_t tellCookedSerialOffset() {
            return tell() - packageHeader_.PackageSummary.HeaderSize;
        }

        [[nodiscard]] inline const FZenPackageSummary& summary() const {
            return packageHeader_.PackageSummary;
        }

        [[nodiscard]] inline const FNameMap& nameMap() const {
            return packageHeader_.NameMap;
        }

        [[nodiscard]] inline std::string getNameString(const FMappedName& mappedName) const {
            return packageHeader_.NameMap.GetName(mappedName).toString();
        }

        [[nodiscard]] inline const std::vector<FBulkDataMapEntry>& bulkDataMap() const {
            return packageHeader_.BulkDataMap;
        }

        [[nodiscard]] inline const std::vector<uint64_t>& importedPublicExportHashes() const {
            return packageHeader_.ImportedPublicExportHashes;
        }

        [[nodiscard]] inline const std::vector<FPackageObjectIndex>& importMap() const {
            return packageHeader_.ImportMap;
        }

        [[nodiscard]] inline const std::vector<FExportMapEntry>& exportMap() const {
            return packageHeader_.ExportMap;
        }

        [[nodiscard]] inline const std::vector<FName>& importedPackageNames() const {
            return packageHeader_.ImportedPackageNames;
        }

        [[nodiscard]] inline bool hasExportMapEntry(const std::string& name) const {
            return exportNameIndicesMap_.contains(name);
        }

        [[nodiscard]] inline bool hasExportMapEntry(uint64_t publicExportHash) const {
            return exportHashIndexMap_.contains(publicExportHash);
        }

        [[nodiscard]] const std::vector<std::size_t>& getExportMapIndicesByName(const std::string& name) const;
        [[nodiscard]] std::optional<std::size_t> getExportMapIndexByHash(uint64_t publicExportHash) const;

        std::shared_ptr<AssetExport> getExportObjectByIdx(std::size_t idx);
        std::shared_ptr<AssetExport> getExportObjectByName(const std::string& name);
        std::shared_ptr<AssetExport> getExportObjectByHash(uint64_t publicExportHash);

        IStreamArchive& ubulkAr() {
            if (ubulk_ar_ == nullptr) {
                throw std::runtime_error("No ubulk data!");
            }
            return *ubulk_ar_;
        }

    protected:
        void serializeName(FName& n) override;

        void serializeObjectReference(FObjectReferenceDisc& ref) override;

        std::shared_ptr<PakManager> pakManager_;
        std::unique_ptr<IStreamArchive> ubulk_ar_;
        FZenPackageHeader packageHeader_;

        std::unordered_map<std::string, std::vector<std::size_t>> exportNameIndicesMap_;
        std::unordered_map<uint64_t, std::size_t> exportHashIndexMap_;
    };
} // namespace SatisfactorySave
