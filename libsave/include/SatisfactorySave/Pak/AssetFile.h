#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../GameTypes/UE/Core/Serialization/MappedName.h"
#include "../GameTypes/UE/CoreUObject/Serialization/AsyncLoading2.h"
#include "../GameTypes/UE/CoreUObject/Serialization/ZenPackageHeader.h"
#include "../IO/Archive/IStreamArchive.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API AssetFile : public IStreamArchive {
    public:
        explicit AssetFile(const std::vector<char>& uassetData, const std::vector<char>& ubulkData = {});

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

        bool hasObjectExportEntry(const std::string& name);
        bool hasObjectExportEntry(uint64_t publicExportHash);

        const FExportMapEntry& getObjectExportEntry(const std::string& name);
        const FExportMapEntry& getObjectExportEntry(uint64_t publicExportHash);

        IStreamArchive& ubulkAr() {
            if (ubulk_ar_ == nullptr) {
                throw std::runtime_error("No ubulk data!");
            }
            return *ubulk_ar_;
        }

    protected:
        void serializeName(FName& n) override;

        void serializeObjectReference(FObjectReferenceDisc& ref) override;

        std::unique_ptr<IStreamArchive> ubulk_ar_;
        FZenPackageHeader packageHeader_;

        std::optional<std::unordered_map<std::string, std::size_t>> exportNameIndexMap_;
        std::optional<std::unordered_map<uint64_t, std::size_t>> exportHashIndexMap_;
    };
} // namespace SatisfactorySave
