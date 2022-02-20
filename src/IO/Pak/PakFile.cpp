#include "PakFile.h"

#include <filesystem>
#include <fstream>

#include "GameTypes/Guid.h"

Satisfactory3DMap::PakFile::PakFile(const std::filesystem::path& pakPath) {
    if (!std::filesystem::is_regular_file(pakPath)) {
        throw std::runtime_error("Pak file invalid: " + pakPath.string());
    }

    pakAr_ = std::make_unique<IFStreamArchive>(pakPath);
    auto& ar = *pakAr_;

    if (ar.size() < 221) {
        throw std::runtime_error("Bad file size!");
    }

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L81-L285
    ar.seek(ar.size() - 221);

    static const uint32_t PakFile_Magic = 0x5A6F12E1;

    FGuid EncryptionKeyGuid;
    ar << EncryptionKeyGuid;
    const int8_t bEncryptedIndex = ar.read<int8_t>();
    const uint32_t Magic = ar.read<uint32_t>();
    if (Magic != PakFile_Magic) {
        throw std::runtime_error("Bad magic!");
    }
    const int32_t Version = ar.read<int32_t>();
    const int64_t IndexOffset = ar.read<int64_t>();
    const int64_t IndexSize = ar.read<int64_t>();
    const std::vector<char> IndexHash = ar.read_vector<char>(20); // FSHAHash
    ar.read_vector<char>(32 * 5);

    if (ar.tell() != ar.size()) {
        throw std::runtime_error("Parse error!");
    }

    ar.seek(IndexOffset);

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Private/IPlatformFilePak.cpp#L5106-L5112
    std::string MountPoint;
    ar << MountPoint;
    const int32_t NumEntries = ar.read<int32_t>();
    const uint64_t PathHashSeed = ar.read<uint64_t>();

    const bool bReaderHasPathHashIndex = !!ar.read<uint32_t>();
    if (!bReaderHasPathHashIndex) {
        throw std::runtime_error("'bReaderHasPathHashIndex == false' not implemented!");
    }
    const int64_t PathHashIndexOffset = ar.read<int64_t>();
    const int64_t PathHashIndexSize = ar.read<int64_t>();
    const std::vector<char> PathHashIndexHash = ar.read_vector<char>(20); // FSHAHash

    const bool bReaderHasFullDirectoryIndex = !!ar.read<uint32_t>();
    if (!bReaderHasFullDirectoryIndex) {
        throw std::runtime_error("'bReaderHasFullDirectoryIndex == false' not implemented!");
    }
    const int64_t FullDirectoryIndexOffset = ar.read<int64_t>();
    const int64_t FullDirectoryIndexSize = ar.read<int64_t>();
    const std::vector<char> FullDirectoryIndexHash = ar.read_vector<char>(20); // FSHAHash

    // EncodedPakEntries
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Private/IPlatformFilePak.cpp#L6007-L6125
    const int32_t EncodedPakEntriesSize = ar.read<int32_t>();
    EncodedPakEntries = ar.read_vector<char>(EncodedPakEntriesSize);

    int32_t FilesNum = ar.read<int32_t>();
    if (FilesNum != 0) {
        throw std::runtime_error("'FilesNum != 0' not implemented!");
    }

    // Validation
    if (ar.tell() != IndexOffset + IndexSize) {
        throw std::runtime_error("Bad index size!");
    }

    // Parse full directory index. Structure is TMap<FString, TMap<FString, int32>>.
    ar.seek(FullDirectoryIndexOffset);
    const int32_t FullDirectoryIndexNum = ar.read<int32_t>();
    for (int32_t i = 0; i < FullDirectoryIndexNum; i++) {
        std::string directoryName;
        ar << directoryName;
        const uint32_t fileNum = ar.read<int32_t>();
        for (int32_t j = 0; j < fileNum; j++) {
            std::string fileName;
            ar << fileName;
            const int32_t fileEntryIdx = ar.read<int32_t>();
            const std::string fullFileName = directoryName + fileName;
            if (directoryEntries_.count(fullFileName) > 0) {
                throw std::runtime_error("Directory filename entry is not unique!");
            }
            directoryEntries_[fullFileName] = fileEntryIdx;
        }
    }

    // Validation
    if (ar.tell() != FullDirectoryIndexOffset + FullDirectoryIndexSize) {
        throw std::runtime_error("Bad full directory index size!");
    }
}

std::vector<std::string> Satisfactory3DMap::PakFile::getAllAssetFilenames() const {
    std::vector<std::string> filenames;
    filenames.reserve(directoryEntries_.size());
    for (const auto& entry : directoryEntries_) {
        filenames.emplace_back(entry.first);
    }
    return filenames;
}

Satisfactory3DMap::AssetFile Satisfactory3DMap::PakFile::readAsset(const std::string& filename) {
    const std::string filenameBase = filename.substr(0, filename.size() - 6);
    const std::string filenameUexp = filenameBase + "uexp";
    const std::string filenameUbulk = filenameBase + "ubulk";
    if (!containsAssetFilename(filenameUexp)) {
        throw std::runtime_error("uexp file missing!");
    }

    const auto uassetFile = readAssetFileContent(filename);
    const auto uexpFile = readAssetFileContent(filenameUexp);
    const auto ubulkFile =
        containsAssetFilename(filenameUbulk) ? readAssetFileContent(filenameUbulk) : std::vector<char>();

    return AssetFile(uassetFile, uexpFile, ubulkFile);
}

std::vector<char> Satisfactory3DMap::PakFile::readAssetFileContent(const std::string& filename) {
    if (directoryEntries_.count(filename) == 0) {
        throw std::runtime_error("Asset file not found in pak: " + filename);
    }

    const auto entry = decodePakEntry(directoryEntries_.at(filename));

    auto& ar = *pakAr_;
    ar.seek(entry.Offset);

    // file header
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Public/IPlatformFilePak.h#L437-L492
    const int64_t Offset = ar.read<int64_t>();
    const int64_t Size = ar.read<int64_t>();
    const int64_t UncompressedSize = ar.read<int64_t>();
    const uint32_t CompressionMethodIndex = ar.read<uint32_t>();
    const std::vector<uint8_t> Hash = ar.read_vector<uint8_t>(20);
    const uint8_t Flags = ar.read<uint8_t>();
    const uint32_t CompressionBlockSize = ar.read<uint32_t>();

    // read asset file
    return ar.read_vector<char>(entry.UncompressedSize);
}

Satisfactory3DMap::PakFile::PakEntry Satisfactory3DMap::PakFile::decodePakEntry(int32_t offset) const {
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
