#include "Pak/PakFile.h"

#include <filesystem>
#include <fstream>

#include "IO/ZlibUtils.h"
#include "Utils/StringUtils.h"

SatisfactorySave::PakFile::PakFile(const std::filesystem::path& pakPath) : NumEntries(0), PathHashSeed(0) {
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
            if (directoryEntries_.contains(fullFileName)) {
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

std::vector<std::string> SatisfactorySave::PakFile::getAllAssetFilenames() const {
    std::vector<std::string> filenames;
    filenames.reserve(directoryEntries_.size());
    for (const auto& entry : directoryEntries_) {
        filenames.emplace_back(entry.first);
    }
    return filenames;
}

SatisfactorySave::AssetFile SatisfactorySave::PakFile::readAsset(const std::string& filename) {
    std::string filenameBase;
    if (filename.ends_with(".uasset")) {
        filenameBase = filename.substr(0, filename.size() - 6);
    } else if (filename.ends_with(".umap")) {
        filenameBase = filename.substr(0, filename.size() - 4);
    } else {
        throw std::runtime_error("Unknown asset extension!");
    }
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

std::vector<char> SatisfactorySave::PakFile::readAssetFileContent(const std::string& filename) {
    if (!directoryEntries_.contains(filename)) {
        throw std::runtime_error("Asset file not found in pak: " + filename);
    }

    const auto entry = decodePakEntry(directoryEntries_.at(filename));

    auto& ar = *pakAr_;
    ar.seek(entry.Offset);

    if (entry.CompressionMethodIndex > 0) {
        if (Info.CompressionMethods[entry.CompressionMethodIndex - 1] == "Zlib") {
            std::vector<char> buffer(entry.UncompressedSize);
            int64_t bufferPos = 0;
            for (const auto& block : entry.CompressionBlocks) {
                auto blockSize = block.CompressedEnd - block.CompressedStart;
                ar.seek(entry.Offset + block.CompressedStart);
                const auto& compressedBuffer = ar.read_buffer(blockSize);
                int64_t destLen =
                    std::min(entry.UncompressedSize - bufferPos, static_cast<int64_t>(entry.CompressionBlockSize));
                zlibUncompress(buffer.data() + bufferPos, destLen, compressedBuffer.data(), compressedBuffer.size());
                bufferPos += destLen;
            }
            return buffer;
        } else {
            throw std::runtime_error("Unknown compression method!");
        }
    } else {
        // file header
        FPakEntry pakEntry;
        ar << pakEntry;

        // read asset file
        return ar.read_buffer(entry.UncompressedSize);
    }
}

SatisfactorySave::FPakEntry SatisfactorySave::PakFile::decodePakEntry(int32_t offset) const {
    // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/PakFile/Private/IPlatformFilePak.cpp#L6007

    FPakEntry entry;

    const uint32_t Value = *reinterpret_cast<const uint32_t*>(EncodedPakEntries.data() + offset);
    offset += sizeof(uint32_t);

    entry.CompressionMethodIndex = (Value >> 23) & 0x3F;

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

    if (entry.CompressionMethodIndex != 0) {
        bool bIsSize32BitSafe = (Value & (1 << 29)) != 0;
        if (bIsSize32BitSafe) {
            entry.Size = static_cast<int64_t>(*reinterpret_cast<const uint32_t*>(EncodedPakEntries.data() + offset));
            offset += sizeof(uint32_t);
        } else {
            entry.Size = *reinterpret_cast<const int64_t*>(EncodedPakEntries.data() + offset);
            offset += sizeof(int64_t);
        }
    } else {
        entry.Size = entry.UncompressedSize;
    }

    const bool EntryEncrypted = (Value & (1 << 22)) != 0;
    if (EntryEncrypted) {
        throw std::runtime_error("Encrypted entry not implemented!");
    }

    uint32_t CompressionBlocksCount = (Value >> 6) & 0xFFFF;
    entry.CompressionBlocks.clear();
    entry.CompressionBlocks.resize(CompressionBlocksCount);

    entry.CompressionBlockSize = 0;
    if (CompressionBlocksCount > 0) {
        entry.CompressionBlockSize =
            entry.UncompressedSize < 65536 ? static_cast<uint32_t>(entry.UncompressedSize) : ((Value & 0x3F) << 11);
    }

    if (entry.CompressionBlocks.size() == 1) {
        FPakCompressedBlock& CompressedBlock = entry.CompressionBlocks[0];
        CompressedBlock.CompressedStart = entry.GetSerializedSize();
        CompressedBlock.CompressedEnd = CompressedBlock.CompressedStart + entry.Size;
    } else if (!entry.CompressionBlocks.empty()) {
        int64_t CompressedBlockOffset = entry.GetSerializedSize();
        for (auto& CompressedBlock : entry.CompressionBlocks) {
            CompressedBlock.CompressedStart = CompressedBlockOffset;
            CompressedBlock.CompressedEnd =
                CompressedBlockOffset + *reinterpret_cast<const int32_t*>(EncodedPakEntries.data() + offset);
            offset += sizeof(int32_t);
            CompressedBlockOffset += CompressedBlock.CompressedEnd - CompressedBlock.CompressedStart;
        }
    }

    return entry;
}
