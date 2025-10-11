#include "Pak/AbstractPakFile.h"

std::unique_ptr<SatisfactorySave::IStream> SatisfactorySave::AbstractPakFile::getAssetFileStream(
    const std::string& filename) {
    return std::make_unique<MemoryBufferIStream>(std::move(readAssetFileContent(filename)));
}

SatisfactorySave::AssetFile SatisfactorySave::AbstractPakFile::readAsset(const std::string& filename) {
    std::string filenameBase;
    if (filename.ends_with(".uasset")) {
        filenameBase = filename.substr(0, filename.size() - 6);
    } else if (filename.ends_with(".umap")) {
        filenameBase = filename.substr(0, filename.size() - 4);
    } else {
        throw std::runtime_error("Unknown asset extension!");
    }
    const std::string filenameUbulk = filenameBase + "ubulk";

    auto uassetFile = getAssetFileStream(filename);
    auto ubulkFile = containsAssetFilename(filenameUbulk) ? getAssetFileStream(filenameUbulk) : nullptr;

    return AssetFile(pakManager_, std::move(uassetFile), std::move(ubulkFile));
}
