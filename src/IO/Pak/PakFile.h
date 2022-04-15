#ifndef SATISFACTORY3DMAP_PAKUTIL_H
#define SATISFACTORY3DMAP_PAKUTIL_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "AssetFile.h"
#include "GameTypes/Guid.h"
#include "GameTypes/Hash.h"
#include "GameTypes/Name.h"
#include "IO/Archive/IStreamArchive.h"

namespace Satisfactory3DMap {
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L81-L285
    struct FPakInfo {
        enum {
            PakFile_Magic = 0x5A6F12E1,
            MaxChunkDataSize = 64 * 1024,
            CompressionMethodNameLen = 32,
            MaxNumCompressionMethods = 5,
        };

        uint32_t Magic = PakFile_Magic;
        int32_t Version = 0;
        int64_t IndexOffset = -1;
        int64_t IndexSize = 0;
        FSHAHash IndexHash;
        uint8_t bEncryptedIndex = 0;
        FGuid EncryptionKeyGuid;
        std::vector<FName> CompressionMethods;

        void serialize(Archive& ar) {
            ar << EncryptionKeyGuid;
            ar << bEncryptedIndex;
            ar << Magic;
            if (Magic != FPakInfo::PakFile_Magic) {
                Magic = 0;
            }
            ar << Version;
            ar << IndexOffset;
            ar << IndexSize;
            ar << IndexHash;

            if (ar.isIArchive()) {
                CompressionMethods.clear();
                int32_t BufferSize = CompressionMethodNameLen * MaxNumCompressionMethods;
                std::vector<char> Methods(BufferSize);
                ar.serializeRaw(Methods.data(), BufferSize);
                for (int32_t i = 0; i < MaxNumCompressionMethods; i++) {
                    FName name;
                    name.Name = std::string(&Methods[i * CompressionMethodNameLen]);
                    CompressionMethods.push_back(name);
                }
            } else {
                throw std::runtime_error("FPakInfo write not implemented!");
            }
        }
    };

    class PakFile {
    public:
        explicit PakFile(const std::filesystem::path& pakPath);

        [[nodiscard]] std::vector<std::string> getAllAssetFilenames() const;

        [[nodiscard]] inline bool containsAssetFilename(const std::string& filename) const {
            return directoryEntries_.count(filename) > 0;
        }

        AssetFile readAsset(const std::string& filename);

        std::vector<char> readAssetFileContent(const std::string& filename);

    private:
        struct PakEntry {
            int64_t Offset = -1;
            int64_t UncompressedSize = 0;
        };

        struct DirectoryEntry {
            std::string filename;
            int32_t entryIdx;
        };

        [[nodiscard]] PakEntry decodePakEntry(int32_t offset) const;

        std::unique_ptr<IFStreamArchive> pakAr_;

        FPakInfo Info;
        std::string MountPoint;
        int32_t NumEntries;
        uint64_t PathHashSeed;
        std::vector<uint8_t> EncodedPakEntries;

        std::unordered_map<std::string, int32_t> directoryEntries_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_PAKUTIL_H
