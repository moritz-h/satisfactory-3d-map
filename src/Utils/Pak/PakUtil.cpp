#include "PakUtil.h"

#include <filesystem>
#include <fstream>

#include "SaveGame/Types/Guid.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::PakUtil::PakUtil() {
    std::filesystem::path gameDir(R"(C:\Program Files (x86)\Steam\steamapps\common\Satisfactory)");
    std::filesystem::path pakPathRelative("FactoryGame/Content/Paks/FactoryGame-WindowsNoEditor.pak");

    std::filesystem::path pakPath = gameDir / pakPathRelative;
    if (!std::filesystem::is_regular_file(pakPath)) {
        throw std::runtime_error("Pak file not found!");
    }
    pakPath = std::filesystem::canonical(pakPath);

    std::ifstream pakFile(pakPath, std::ios::binary);
    if (!pakFile.is_open()) {
        throw std::runtime_error("Cannot read pak file!");
    }

    // File size
    pakFile.seekg(0, std::ios::end);
    const auto filesize = pakFile.tellg();
    pakFile.seekg(0, std::ios::beg);

    if (filesize < 221) {
        throw std::runtime_error("Bad file size!");
    }

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L81-L285
    pakFile.seekg(-221, std::ios::end);

    static const uint32_t PakFile_Magic = 0x5A6F12E1;

    const Guid EncryptionKeyGuid(pakFile);
    const int8_t bEncryptedIndex = read<int8_t>(pakFile);
    const uint32_t Magic = read<uint32_t>(pakFile);
    if (Magic != PakFile_Magic) {
        throw std::runtime_error("Bad magic!");
    }
    const int32_t Version = read<int32_t>(pakFile);
    const int64_t IndexOffset = read<int64_t>(pakFile);
    const int64_t IndexSize = read<int64_t>(pakFile);
    const std::vector<char> IndexHash = read_vector<char>(pakFile, 20); // FSHAHash
    read_vector<char>(pakFile, 32 * 5);

    if (pakFile.tellg() != filesize) {
        throw std::runtime_error("Parse error!");
    }

    pakFile.seekg(IndexOffset, std::ios::beg);

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Private/IPlatformFilePak.cpp#L5106-L5112
    const std::string MountPoint = read_length_string(pakFile);
    const int32_t NumEntries = read<int32_t>(pakFile);
    const uint64_t PathHashSeed = read<uint64_t>(pakFile);

    const bool bReaderHasPathHashIndex = !!read<uint32_t>(pakFile);
    if (bReaderHasPathHashIndex) {
        const int64_t PathHashIndexOffset = read<int64_t>(pakFile);
        const int64_t PathHashIndexSize = read<int64_t>(pakFile);
        const std::vector<char> PathHashIndexHash = read_vector<char>(pakFile, 20); // FSHAHash
    }

    const bool bReaderHasFullDirectoryIndex = !!read<uint32_t>(pakFile);
    if (!bReaderHasFullDirectoryIndex) {
        throw std::runtime_error("'bReaderHasFullDirectoryIndex == false' not implemented!");
    }
    const int64_t FullDirectoryIndexOffset = read<int64_t>(pakFile);
    const int64_t FullDirectoryIndexSize = read<int64_t>(pakFile);
    const std::vector<char> FullDirectoryIndexHash = read_vector<char>(pakFile, 20); // FSHAHash

    // EncodedPakEntries
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Private/IPlatformFilePak.cpp#L6007-L6125
    const int32_t EncodedPakEntriesSize = read<int32_t>(pakFile);
    for (int32_t i = 0; i < NumEntries; i++) {
        const uint32_t Value = read<uint32_t>(pakFile);

        const uint32_t EntryCompressionMethodIndex = (Value >> 23) & 0x3f;
        if (EntryCompressionMethodIndex != 0) {
            throw std::runtime_error("'EntryCompressionMethodIndex != 0' not implemented!");
        }
        const bool EntryEncrypted = (Value & (1 << 22)) != 0;
        if (EntryEncrypted) {
            throw std::runtime_error("Encrypted entry not implemented!");
        }

        PakEntry entry;

        bool bIsOffset32BitSafe = (Value & (1 << 31)) != 0;
        if (bIsOffset32BitSafe) {
            entry.Offset = static_cast<int64_t>(read<uint32_t>(pakFile));
        } else {
            entry.Offset = read<int64_t>(pakFile);
        }

        bool bIsUncompressedSize32BitSafe = (Value & (1 << 30)) != 0;
        if (bIsUncompressedSize32BitSafe) {
            entry.UncompressedSize = static_cast<int64_t>(read<uint32_t>(pakFile));
        } else {
            entry.UncompressedSize = read<int64_t>(pakFile);
        }
        pakEntries_.emplace_back(entry);
    }

    int32_t FilesNum = read<int32_t>(pakFile);
    if (FilesNum != 0) {
        throw std::runtime_error("'FilesNum != 0' not implemented!");
    }

    // Validation
    if (pakFile.tellg() != IndexOffset + IndexSize) {
        throw std::runtime_error("Bad index size!");
    }

    // Parse full directory index. Structure is TMap<FString, TMap<FString, int32>>.
    pakFile.seekg(FullDirectoryIndexOffset, std::ios::beg);
    const int32_t FullDirectoryIndexNum = read<int32_t>(pakFile);
    for (int32_t i = 0; i < FullDirectoryIndexNum; i++) {
        const std::string directoryName = read_length_string(pakFile);
        const uint32_t fileNum = read<int32_t>(pakFile);
        for (int32_t j = 0; j < fileNum; j++) {
            const std::string fileName = read_length_string(pakFile);
            const int32_t fileEntryIdx = read<int32_t>(pakFile);
            const std::string fullFileName = directoryName + fileName;
            if (directoryEntries_.count(fullFileName) > 0) {
                throw std::runtime_error("Directory filename entry is not unique!");
            }
            directoryEntries_[fullFileName] = fileEntryIdx;
        }
    }

    // Validation
    if (pakFile.tellg() != FullDirectoryIndexOffset + FullDirectoryIndexSize) {
        throw std::runtime_error("Bad full directory index size!");
    }
}
