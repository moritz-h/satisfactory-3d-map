#include "Pak/IoStoreFile.h"

#include <cstring>

#include "GameTypes/UE/Core/IO/IoStore.h"
#include "IO/MemoryStreams.h"

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

SatisfactorySave::IoStoreFile::IoStoreFile(const std::filesystem::path& path) {
    if (!std::filesystem::is_regular_file(path)) {
        throw std::runtime_error("IoStore file invalid: " + path.string());
    }

    IFStreamArchive utocAr(path);
    utocAr << utoc_;

    // Debug validation
    if (utocAr.tell() != utocAr.size()) {
        throw std::runtime_error("utoc file not fully read!");
    }

    if (!utoc_.DirectoryIndexBuffer.empty()) {
        auto buf = std::make_unique<std::vector<char>>();
        buf->resize(utoc_.DirectoryIndexBuffer.size());
        std::memcpy(buf->data(), utoc_.DirectoryIndexBuffer.data(), utoc_.DirectoryIndexBuffer.size());
        IStreamArchive dirIndexAr(std::make_unique<MemIStream>(std::move(buf)));

        FIoDirectoryIndexResource dirIndexRes;
        dirIndexAr << dirIndexRes;

        dirIndex_ = std::make_unique<DirectoryIndexReader>(dirIndexRes);
    }
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
    // TODO
    return {};
}
