#pragma once

#include <optional>
#include <unordered_map>
#include <vector>

#include "../GameTypes/UE/CoreUObject/UObject/ObjectResource.h"
#include "../IO/Archive/IStreamArchive.h"
#include "Serialization/ObjectExport.h"
#include "Serialization/ObjectImport.h"
#include "Serialization/PackageFileSummary.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    struct SATISFACTORYSAVE_API NameEntrySerialized {
        std::string Name;

        void serialize(Archive& ar) {
            ar << Name;
            uint16_t DummyHashes[2]{0, 0};
            ar << DummyHashes[0];
            ar << DummyHashes[1];
        }
    };

    class SATISFACTORYSAVE_API AssetFile : public IStreamArchive {
    public:
        AssetFile(const std::vector<char>& uassetData, const std::vector<char>& uexpData,
            const std::vector<char>& ubulkData = {});

        [[nodiscard]] inline const PackageFileSummary& summary() const {
            return summary_;
        }

        [[nodiscard]] std::string nameMapToString() const;

        [[nodiscard]] inline const std::vector<ObjectImport>& importMap() const {
            return importMap_;
        }

        [[nodiscard]] inline const std::vector<ObjectExport>& exportMap() const {
            return exportMap_;
        }

        bool hasObjectExportEntry(const std::string& name);

        const ObjectExport& getObjectExportEntry(const std::string& name);

        std::vector<FObjectDataResource> DataResourceMap;

    protected:
        // Extra struct to serialize FName index entries
        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Public/UObject/LinkerLoad.h#L854-L878
        struct NameEntry {
            int32_t NameIndex = 0;
            int32_t Number = 0;

            void serialize(Archive& ar) {
                ar << NameIndex;
                ar << Number;
            }
        };

        void serializeName(FName& n) override;

        void serializeObjectReference(FObjectReferenceDisc& ref) override;

        PackageFileSummary summary_;
        std::vector<NameEntrySerialized> nameMap_;
        std::vector<ObjectImport> importMap_;
        std::vector<ObjectExport> exportMap_;

        std::optional<std::unordered_map<std::string, std::size_t>> exportNameIndexMap_;
    };
} // namespace SatisfactorySave
