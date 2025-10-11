#include "Pak/IoStoreFile.h"

#include "GameTypes/UE/Core/IO/IoStore.h"
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

SatisfactorySave::IoStoreFile::IoStoreFile(Private p, std::shared_ptr<PakManager> pakManager,
    const std::filesystem::path& path)
    : AbstractPakFile(p, std::move(pakManager)) {
    if (!std::filesystem::is_regular_file(path)) {
        throw std::runtime_error("IoStore utoc file invalid: " + path.string());
    }
    std::filesystem::path ucas_path = path;
    ucas_path.replace_extension(std::filesystem::path(".ucas"));
    if (!std::filesystem::is_regular_file(ucas_path)) {
        throw std::runtime_error("IoStore ucas file invalid: " + ucas_path.string());
    }

    IStreamArchive utocAr(path);
    utocAr << utoc_;

    // Debug validation
    if (utocAr.tell() != utocAr.size()) {
        throw std::runtime_error("utoc file not fully read!");
    }

    if (!utoc_.DirectoryIndexBuffer.empty()) {
        IStreamArchive dirIndexAr(vector_to_char_span(utoc_.DirectoryIndexBuffer));

        FIoDirectoryIndexResource dirIndexRes;
        dirIndexAr << dirIndexRes;

        dirIndex_ = std::make_unique<DirectoryIndexReader>(dirIndexRes);
    }

    ucasAr_ = std::make_unique<IStreamArchive>(ucas_path);
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
    return readChunkContent(getChunkIdx(filename));
}

std::unique_ptr<SatisfactorySave::IStream> SatisfactorySave::IoStoreFile::getAssetFileStream(
    const std::string& filename) {
    return std::make_unique<AssetFileIStream>(std::static_pointer_cast<IoStoreFile>(shared_from_this()),
        getChunkIdx(filename));
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

    // Loop over compression blocks
    for (uint32_t i = 0; i < compBlockNum; i++) {
        const uint32_t expectedSize =
            (i < compBlockNum - 1) ? blockSize : chunk.GetLength() - (compBlockNum - 1) * blockSize;
        readCompressionBlock(compBlockIdx + i, buf.data() + i * blockSize, expectedSize);
    }

    return buf;
}

uint32_t SatisfactorySave::IoStoreFile::getChunkIdx(const std::string& filename) const {
    if (dirIndex_ == nullptr) {
        throw std::runtime_error("Missing directory index!");
    }
    if (!dirIndex_->directoryEntries().contains(filename)) {
        throw std::runtime_error("Asset file not found in utoc: " + filename);
    }

    // UserData of directory index file entry is index to chunk data
    return dirIndex_->directoryEntries().at(filename);
}

void SatisfactorySave::IoStoreFile::readCompressionBlock(uint64_t blockIdx, char* dst, uint32_t expectedSize) {
    auto& ar = *ucasAr_;
    const auto& compBlock = utoc_.CompressionBlocks[blockIdx];

    // Validate that uncompressed size of all compression blocks (except the last one) is block size.
    if (compBlock.GetUncompressedSize() != expectedSize) {
        throw std::runtime_error("Unexpected uncompressed block size");
    }

    ar.seek(compBlock.GetOffset());
    const auto compMethod = utoc_.CompressionMethods.at(compBlock.GetCompressionMethodIndex());
    if (compMethod == "None") {
        // Uncompressed, copy directly to out buffer.
        ar.serializeRaw(dst, compBlock.GetCompressedSize());
    } else if (compMethod == "Oodle") {
        const auto comp_buf = ar.read_buffer(compBlock.GetCompressedSize());
        oodleDecompress(dst, compBlock.GetUncompressedSize(), comp_buf.data(), compBlock.GetCompressedSize());
    } else {
        throw std::runtime_error("Unknown compression method: " + compMethod);
    }
}
