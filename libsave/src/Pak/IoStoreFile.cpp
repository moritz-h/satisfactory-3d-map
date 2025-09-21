#include "Pak/IoStoreFile.h"

#include "GameTypes/UE/Core/IO/IoStore.h"
#include "IO/MemoryStreams.h"
#include "IO/OodleUtils.h"
#include "Utils/VectorUtils.h"

SatisfactorySave::DirectoryIndexReader::DirectoryIndexReader(const FIoDirectoryIndexResource& res) : res_(res) {
    if (!res_.MountPoint.starts_with("../../../")) {
        throw std::runtime_error("Invalid mount point");
    }
    parseDir(0, res_.MountPoint.substr(9));
}

void SatisfactorySave::DirectoryIndexReader::parseDir(uint32_t dir_idx, const std::string& path) {
    if (dir_idx == none) {
        return;
    }
    const auto& entry = res_.DirectoryEntries[dir_idx];
    const std::string name = (entry.Name != none) ? (res_.StringTable[entry.Name] + "/") : "";
    // children
    parseDir(entry.FirstChildEntry, path + name);
    // siblings
    parseDir(entry.NextSiblingEntry, path);
    // files
    parseFile(entry.FirstFileEntry, path + name);
}

void SatisfactorySave::DirectoryIndexReader::parseFile(uint32_t file_idx, const std::string& path) {
    if (file_idx == none) {
        return;
    }
    const auto& entry = res_.FileEntries[file_idx];
    const std::string filename = path + res_.StringTable[entry.Name];
    if (directoryEntries_.contains(filename)) {
        throw std::runtime_error("Directory filename entry is not unique!");
    }
    directoryEntries_[filename] = entry.UserData;

    parseFile(entry.NextFileEntry, path);
}

SatisfactorySave::IoStoreFile::IoStoreFile(std::shared_ptr<PakManager> pakManager, const std::filesystem::path& path)
    : AbstractPakFile(std::move(pakManager)) {
    if (!std::filesystem::is_regular_file(path)) {
        throw std::runtime_error("IoStore utoc file invalid: " + path.string());
    }
    std::filesystem::path ucas_path = path;
    ucas_path.replace_extension(std::filesystem::path(".ucas"));
    if (!std::filesystem::is_regular_file(ucas_path)) {
        throw std::runtime_error("IoStore ucas file invalid: " + ucas_path.string());
    }

    IFStreamArchive utocAr(path);
    utocAr << utoc_;

    // Debug validation
    if (utocAr.tell() != utocAr.size()) {
        throw std::runtime_error("utoc file not fully read!");
    }

    if (!utoc_.DirectoryIndexBuffer.empty()) {
        IStreamArchive dirIndexAr(std::make_unique<MemIStream>(vector_to_char_span(utoc_.DirectoryIndexBuffer)));

        FIoDirectoryIndexResource dirIndexRes;
        dirIndexAr << dirIndexRes;

        dirIndex_ = std::make_unique<DirectoryIndexReader>(dirIndexRes);
    }

    ucasAr_ = std::make_unique<IFStreamArchive>(ucas_path);
}

std::vector<std::string> SatisfactorySave::IoStoreFile::getAllAssetFilenames() const {
    if (dirIndex_ == nullptr) {
        return {};
    }

    std::vector<std::string> filenames;
    filenames.reserve(dirIndex_->directoryEntries().size());
    for (const auto& entry : dirIndex_->directoryEntries()) {
        filenames.emplace_back(entry.first);
    }
    return filenames;
}

std::vector<char> SatisfactorySave::IoStoreFile::readAssetFileContent(const std::string& filename) {
    if (dirIndex_ == nullptr) {
        throw std::runtime_error("Missing directory index!");
    }
    if (!dirIndex_->directoryEntries().contains(filename)) {
        throw std::runtime_error("Asset file not found in utoc: " + filename);
    }

    // UserData of directory index file entry is index to chunk data
    uint32_t chunkIdx = dirIndex_->directoryEntries().at(filename);

    return readChunkContent(chunkIdx);
}

std::vector<char> SatisfactorySave::IoStoreFile::readChunkContent(std::size_t chunkIdx) {
    const auto& chunk = utoc_.ChunkOffsetLengths[chunkIdx];

    // The idea behind chunk data seems to be that all data is stored in a single address space. The address space is
    // divided into blocks. All data seems aligned to a block start. Each block is then compressed individually. The
    // ucas stores the compressed blocks directly after each other. Divide address space offset and length of chunk
    // data by the block size to get index and number of compression blocks.
    const auto& blockSize = utoc_.Header.CompressionBlockSize;
    const uint64_t compBlockIdx = chunk.GetOffset() / blockSize;
    const uint32_t compBlockNum = (chunk.GetLength() + blockSize - 1) / blockSize;

    // Allocate output
    std::vector<char> buf(chunk.GetLength());

    auto& ar = *ucasAr_;

    // Loop over compression blocks
    for (uint32_t i = 0; i < compBlockNum; i++) {
        const auto& compBlock = utoc_.CompressionBlocks[compBlockIdx + i];

        // Validate that uncompressed size of all compression blocks (except the last one) is block size.
        if (i < compBlockNum - 1 && compBlock.GetUncompressedSize() != blockSize) {
            throw std::runtime_error("Unexpected uncompressed block size");
        }

        ar.seek(compBlock.GetOffset());
        const auto compMethod = utoc_.CompressionMethods.at(compBlock.GetCompressionMethodIndex());
        if (compMethod == "None") {
            // Uncompressed, copy directly to out buffer.
            ar.serializeRaw(buf.data() + i * blockSize, compBlock.GetCompressedSize());
        } else if (compMethod == "Oodle") {
            const auto comp_buf = ar.read_buffer(compBlock.GetCompressedSize());
            oodleDecompress(buf.data() + i * blockSize, compBlock.GetUncompressedSize(), comp_buf.data(),
                compBlock.GetCompressedSize());
        } else {
            throw std::runtime_error("Unknown compression method: " + compMethod);
        }
    }

    return buf;
}
