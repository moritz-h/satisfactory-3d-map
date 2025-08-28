#include "Pak/AbstractPakFile.h"

SatisfactorySave::AssetFile SatisfactorySave::AbstractPakFile::readAsset(const std::string& filename) {
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
