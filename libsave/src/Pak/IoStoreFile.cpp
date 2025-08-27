#include "Pak/IoStoreFile.h"

#include "GameTypes/UE/Core/IO/IoStore.h"

SatisfactorySave::IoStoreFile::IoStoreFile(const std::filesystem::path& path) {
    if (!std::filesystem::is_regular_file(path)) {
        throw std::runtime_error("IoStore file invalid: " + path.string());
    }

    ioStoreAr_ = std::make_unique<IFStreamArchive>(path);
    auto& ar = *ioStoreAr_;

    FIoStoreTocResource utoc;
    ar << utoc;

    // Debug validation
    if (ar.tell() != ar.size()) {
        throw std::runtime_error("utoc file not fully read!");
    }
}
