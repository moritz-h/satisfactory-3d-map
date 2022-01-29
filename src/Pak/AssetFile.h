#ifndef SATISFACTORY3DMAP_ASSETUTIL_H
#define SATISFACTORY3DMAP_ASSETUTIL_H

#include <vector>

#include "GameTypes/Name.h"
#include "GameTypes/Serialization/ObjectExport.h"
#include "GameTypes/Serialization/ObjectImport.h"
#include "GameTypes/Serialization/PackageFileSummary.h"
#include "IO/Archive/IStreamArchive.h"

namespace Satisfactory3DMap {
    struct NameEntrySerialized {
        std::string Name;

        void serialize(Archive& ar) {
            ar << Name;
            uint16_t DummyHashes[2]{0, 0};
            ar << DummyHashes[0];
            ar << DummyHashes[1];
        }
    };

    class AssetFile : public IStreamArchive {
    public:
        AssetFile(const std::vector<char>& uassetData, const std::vector<char>& uexpData);

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

        PackageFileSummary summary_;
        std::vector<NameEntrySerialized> nameMap_;
        std::vector<ObjectImport> importMap_;
        std::vector<ObjectExport> exportMap_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ASSETUTIL_H
