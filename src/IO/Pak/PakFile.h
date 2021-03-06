#pragma once

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

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L437-L492
    struct FPakEntry {
        int64_t Offset = -1;
        int64_t Size = 0;
        int64_t UncompressedSize = 0;
        std::array<uint8_t, 20> Hash{};
        // TArray<FPakCompressedBlock> CompressionBlocks;
        uint32_t CompressionBlockSize = 0;
        uint32_t CompressionMethodIndex = 0;
        uint8_t Flags = 0;

        void serialize(Archive& ar) {
            ar << Offset;
            ar << Size;
            ar << UncompressedSize;
            ar << CompressionMethodIndex;
            ar.serializeRaw(Hash.data(), 20 * sizeof(uint8_t));
            if (CompressionMethodIndex != 0) {
                throw std::runtime_error("CompressionMethodIndex != 0 not implemented");
                // ar << CompressionBlocks;
            }
            ar << Flags;
            ar << CompressionBlockSize;
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
        // Unreal uses FPakEntry do decode EncodedPakEntries, but we only need two values from it. Therefore, we omit
        // full decoding and use a smaller type with only two entries.
        struct SmallPakEntry {
            int64_t Offset = -1;
            int64_t UncompressedSize = 0;
        };

        struct DirectoryEntry {
            std::string filename;
            int32_t entryIdx;
        };

        [[nodiscard]] SmallPakEntry decodePakEntry(int32_t offset) const;

        std::unique_ptr<IFStreamArchive> pakAr_;

        FPakInfo Info;
        std::string MountPoint;
        int32_t NumEntries;
        uint64_t PathHashSeed;
        std::vector<uint8_t> EncodedPakEntries;

        std::unordered_map<std::string, int32_t> directoryEntries_;
    };
} // namespace Satisfactory3DMap
