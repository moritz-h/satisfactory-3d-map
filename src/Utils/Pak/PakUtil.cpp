#include "PakUtil.h"

#include <filesystem>
#include <fstream>

#include "GameTypes/Guid.h"
#include "Utils/StreamUtils.h"

Satisfactory3DMap::PakUtil::PakUtil() {
    std::filesystem::path gameDir(R"(C:\Program Files (x86)\Steam\steamapps\common\Satisfactory)");
    std::filesystem::path pakPathRelative("FactoryGame/Content/Paks/FactoryGame-WindowsNoEditor.pak");

    std::filesystem::path pakPath = gameDir / pakPathRelative;
    if (!std::filesystem::is_regular_file(pakPath)) {
        throw std::runtime_error("Pak file not found!");
    }
    pakPath = std::filesystem::canonical(pakPath);

    pakFile_ = std::ifstream(pakPath, std::ios::binary);
    if (!pakFile_.is_open()) {
        throw std::runtime_error("Cannot read pak file!");
    }

    // File size
    pakFile_.seekg(0, std::ios::end);
    const auto filesize = pakFile_.tellg();
    pakFile_.seekg(0, std::ios::beg);

    if (filesize < 221) {
        throw std::runtime_error("Bad file size!");
    }

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L81-L285
    pakFile_.seekg(-221, std::ios::end);

    static const uint32_t PakFile_Magic = 0x5A6F12E1;

    const Guid EncryptionKeyGuid(pakFile_);
    const int8_t bEncryptedIndex = read<int8_t>(pakFile_);
    const uint32_t Magic = read<uint32_t>(pakFile_);
    if (Magic != PakFile_Magic) {
        throw std::runtime_error("Bad magic!");
    }
    const int32_t Version = read<int32_t>(pakFile_);
    const int64_t IndexOffset = read<int64_t>(pakFile_);
    const int64_t IndexSize = read<int64_t>(pakFile_);
    const std::vector<char> IndexHash = read_vector<char>(pakFile_, 20); // FSHAHash
    read_vector<char>(pakFile_, 32 * 5);

    if (pakFile_.tellg() != filesize) {
        throw std::runtime_error("Parse error!");
    }

    pakFile_.seekg(IndexOffset, std::ios::beg);

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Private/IPlatformFilePak.cpp#L5106-L5112
    const std::string MountPoint = read_length_string(pakFile_);
    const int32_t NumEntries = read<int32_t>(pakFile_);
    const uint64_t PathHashSeed = read<uint64_t>(pakFile_);

    const bool bReaderHasPathHashIndex = !!read<uint32_t>(pakFile_);
    if (bReaderHasPathHashIndex) {
        const int64_t PathHashIndexOffset = read<int64_t>(pakFile_);
        const int64_t PathHashIndexSize = read<int64_t>(pakFile_);
        const std::vector<char> PathHashIndexHash = read_vector<char>(pakFile_, 20); // FSHAHash
    }

    const bool bReaderHasFullDirectoryIndex = !!read<uint32_t>(pakFile_);
    if (!bReaderHasFullDirectoryIndex) {
        throw std::runtime_error("'bReaderHasFullDirectoryIndex == false' not implemented!");
    }
    const int64_t FullDirectoryIndexOffset = read<int64_t>(pakFile_);
    const int64_t FullDirectoryIndexSize = read<int64_t>(pakFile_);
    const std::vector<char> FullDirectoryIndexHash = read_vector<char>(pakFile_, 20); // FSHAHash

    // EncodedPakEntries
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Private/IPlatformFilePak.cpp#L6007-L6125
    const int32_t EncodedPakEntriesSize = read<int32_t>(pakFile_);
    EncodedPakEntries = read_vector<char>(pakFile_, EncodedPakEntriesSize);

    int32_t FilesNum = read<int32_t>(pakFile_);
    if (FilesNum != 0) {
        throw std::runtime_error("'FilesNum != 0' not implemented!");
    }

    // Validation
    if (pakFile_.tellg() != IndexOffset + IndexSize) {
        throw std::runtime_error("Bad index size!");
    }

    // Parse full directory index. Structure is TMap<FString, TMap<FString, int32>>.
    pakFile_.seekg(FullDirectoryIndexOffset, std::ios::beg);
    const int32_t FullDirectoryIndexNum = read<int32_t>(pakFile_);
    for (int32_t i = 0; i < FullDirectoryIndexNum; i++) {
        const std::string directoryName = read_length_string(pakFile_);
        const uint32_t fileNum = read<int32_t>(pakFile_);
        for (int32_t j = 0; j < fileNum; j++) {
            const std::string fileName = read_length_string(pakFile_);
            const int32_t fileEntryIdx = read<int32_t>(pakFile_);
            const std::string fullFileName = directoryName + fileName;
            if (directoryEntries_.count(fullFileName) > 0) {
                throw std::runtime_error("Directory filename entry is not unique!");
            }
            directoryEntries_[fullFileName] = fileEntryIdx;
        }
    }

    // Validation
    if (pakFile_.tellg() != FullDirectoryIndexOffset + FullDirectoryIndexSize) {
        throw std::runtime_error("Bad full directory index size!");
    }
}

std::vector<std::string> Satisfactory3DMap::PakUtil::getAllFilenames() const {
    std::vector<std::string> filenames;
    filenames.reserve(directoryEntries_.size());
    for (const auto& entry : directoryEntries_) {
        filenames.emplace_back(entry.first);
    }
    return filenames;
}

std::vector<char> Satisfactory3DMap::PakUtil::readAsset(const std::string& filename) {
    if (directoryEntries_.count(filename) == 0) {
        throw std::runtime_error("Asset file not found in pak: " + filename);
    }

    const auto entry = decodePakEntry(directoryEntries_.at(filename));

    pakFile_.seekg(entry.Offset, std::ios::beg);

    // file header
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L437-L492
    const int64_t Offset = read<int64_t>(pakFile_);
    const int64_t Size = read<int64_t>(pakFile_);
    const int64_t UncompressedSize = read<int64_t>(pakFile_);
    const uint32_t CompressionMethodIndex = read<uint32_t>(pakFile_);
    const std::vector<uint8_t> Hash = read_vector<uint8_t>(pakFile_, 20);
    const uint8_t Flags = read<uint8_t>(pakFile_);
    const uint32_t CompressionBlockSize = read<uint32_t>(pakFile_);

    // read asset file
    return read_vector<char>(pakFile_, entry.UncompressedSize);
}

Satisfactory3DMap::PakUtil::PakEntry Satisfactory3DMap::PakUtil::decodePakEntry(int32_t offset) const {
    PakEntry entry;

    const uint32_t Value = *reinterpret_cast<const uint32_t*>(EncodedPakEntries.data() + offset);
    offset += sizeof(uint32_t);

    const uint32_t EntryCompressionMethodIndex = (Value >> 23) & 0x3f;
    if (EntryCompressionMethodIndex != 0) {
        throw std::runtime_error("'EntryCompressionMethodIndex != 0' not implemented!");
    }
    const bool EntryEncrypted = (Value & (1 << 22)) != 0;
    if (EntryEncrypted) {
        throw std::runtime_error("Encrypted entry not implemented!");
    }

    bool bIsOffset32BitSafe = (Value & (1 << 31)) != 0;
    if (bIsOffset32BitSafe) {
        entry.Offset = static_cast<int64_t>(*reinterpret_cast<const uint32_t*>(EncodedPakEntries.data() + offset));
        offset += sizeof(uint32_t);
    } else {
        entry.Offset = *reinterpret_cast<const int64_t*>(EncodedPakEntries.data() + offset);
        offset += sizeof(int64_t);
    }

    bool bIsUncompressedSize32BitSafe = (Value & (1 << 30)) != 0;
    if (bIsUncompressedSize32BitSafe) {
        entry.UncompressedSize =
            static_cast<int64_t>(*reinterpret_cast<const uint32_t*>(EncodedPakEntries.data() + offset));
        offset += sizeof(uint32_t);
    } else {
        entry.UncompressedSize = *reinterpret_cast<const int64_t*>(EncodedPakEntries.data() + offset);
        offset += sizeof(int64_t);
    }

    return entry;
}
