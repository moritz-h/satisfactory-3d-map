#include "PakFile.h"

#include <filesystem>
#include <fstream>

Satisfactory3DMap::PakFile::PakFile(const std::filesystem::path& pakPath) : NumEntries(0), PathHashSeed(0) {
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

    ar << Info;
    if (Info.Magic != FPakInfo::PakFile_Magic) {
        throw std::runtime_error("Bad magic!");
    }

    if (ar.tell() != ar.size()) {
        throw std::runtime_error("Error parsing Pak info!");
    }

    ar.seek(Info.IndexOffset);

    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Private/IPlatformFilePak.cpp#L5106-L5112
    ar << MountPoint;
    ar << NumEntries;
    ar << PathHashSeed;

    bool bReaderHasPathHashIndex = false;
    int64_t PathHashIndexOffset = -1;
    int64_t PathHashIndexSize = 0;
    FSHAHash PathHashIndexHash;
    ar << bReaderHasPathHashIndex;
    if (bReaderHasPathHashIndex) {
        ar << PathHashIndexOffset;
        ar << PathHashIndexSize;
        ar << PathHashIndexHash;
    }

    bool bReaderHasFullDirectoryIndex = false;
    int64_t FullDirectoryIndexOffset = -1;
    int64_t FullDirectoryIndexSize = 0;
    FSHAHash FullDirectoryIndexHash;
    ar << bReaderHasFullDirectoryIndex;
    if (bReaderHasFullDirectoryIndex) {
        ar << FullDirectoryIndexOffset;
        ar << FullDirectoryIndexSize;
        ar << FullDirectoryIndexHash;
    }

    // EncodedPakEntries
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Private/IPlatformFilePak.cpp#L6007-L6125
    ar << EncodedPakEntries;

    int32_t FilesNum = 0;
    ar << FilesNum;
    if (FilesNum != 0) {
        throw std::runtime_error("'FilesNum != 0' not implemented!");
    }

    // Validation
    if (static_cast<int64_t>(ar.tell()) != Info.IndexOffset + Info.IndexSize) {
        throw std::runtime_error("Bad index size!");
    }

    if (!bReaderHasFullDirectoryIndex) {
        throw std::runtime_error("FullDirectoryIndex missing in pak file!");
    }

    // Parse full directory index. Structure is TMap<FString, TMap<FString, int32>>.
    ar.seek(FullDirectoryIndexOffset);
    const int32_t FullDirectoryIndexNum = ar.read<int32_t>();
    for (int32_t i = 0; i < FullDirectoryIndexNum; i++) {
        std::string directoryName;
        ar << directoryName;
        const uint32_t fileNum = ar.read<int32_t>();
        for (uint32_t j = 0; j < fileNum; j++) {
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
    if (static_cast<int64_t>(ar.tell()) != FullDirectoryIndexOffset + FullDirectoryIndexSize) {
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

    const auto smallEntry = decodePakEntry(directoryEntries_.at(filename));

    auto& ar = *pakAr_;
    ar.seek(smallEntry.Offset);

    // file header
    FPakEntry pakEntry;
    ar << pakEntry;

    // read asset file
    return ar.read_vector<char>(smallEntry.UncompressedSize);
}

Satisfactory3DMap::PakFile::SmallPakEntry Satisfactory3DMap::PakFile::decodePakEntry(int32_t offset) const {
    SmallPakEntry entry;

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
