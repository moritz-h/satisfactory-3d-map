#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "../GameTypes/UE/Core/Misc/Guid.h"
#include "../GameTypes/UE/Core/Misc/SecureHash.h"
#include "../GameTypes/UE/Core/UObject/NameTypes.h"
#include "../IO/Archive/IStreamArchive.h"
#include "AssetFile.h"
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L81-L285
    struct SATISFACTORYSAVE_API FPakInfo {
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
                    CompressionMethods.emplace_back(std::string(&Methods[i * CompressionMethodNameLen]));
                }
            } else {
                throw std::runtime_error("FPakInfo write not implemented!");
            }
        }
    };

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L290-L306
    struct SATISFACTORYSAVE_API FPakCompressedBlock {
        int64_t CompressedStart;
        int64_t CompressedEnd;

        void serialize(Archive& ar) {
            ar << CompressedStart;
            ar << CompressedEnd;
        }
    };

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L437-L492
    struct SATISFACTORYSAVE_API FPakEntry {
        int64_t Offset = -1;
        int64_t Size = 0;
        int64_t UncompressedSize = 0;
        std::array<uint8_t, 20> Hash{};
        std::vector<FPakCompressedBlock> CompressionBlocks;
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
                ar << CompressionBlocks;
            }
            ar << Flags;
            ar << CompressionBlockSize;
        }

        [[nodiscard]] int64_t GetSerializedSize() const {
            int64_t SerializedSize = sizeof(Offset) + sizeof(Size) + sizeof(UncompressedSize) + sizeof(Hash) +
                                     sizeof(CompressionMethodIndex) + sizeof(Flags) + sizeof(CompressionBlockSize);
            if (CompressionMethodIndex != 0) {
                SerializedSize += sizeof(FPakCompressedBlock) * CompressionBlocks.size() + sizeof(int32_t);
            }
            return SerializedSize;
        }
    };

    class SATISFACTORYSAVE_API PakFile {
    public:
        explicit PakFile(const std::filesystem::path& pakPath);

        [[nodiscard]] std::vector<std::string> getAllAssetFilenames() const;

        [[nodiscard]] inline bool containsAssetFilename(const std::string& filename) const {
            return directoryEntries_.contains(filename);
        }

        AssetFile readAsset(const std::string& filename);

        std::vector<char> readAssetFileContent(const std::string& filename);

    private:
        struct DirectoryEntry {
            std::string filename;
            int32_t entryIdx;
        };

        [[nodiscard]] FPakEntry decodePakEntry(int32_t offset) const;

        std::unique_ptr<IFStreamArchive> pakAr_;

        FPakInfo Info;
        std::string MountPoint;
        int32_t NumEntries;
        uint64_t PathHashSeed;
        std::vector<uint8_t> EncodedPakEntries;

        std::unordered_map<std::string, int32_t> directoryEntries_;
    };
} // namespace SatisfactorySave
