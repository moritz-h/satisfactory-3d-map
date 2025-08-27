#include "Pak/PakManager.h"

#include <spdlog/spdlog.h>

#include "Utils/StringUtils.h"

SatisfactorySave::PakManager::PakManager(const std::filesystem::path& gameDir) {
    spdlog::info("PakManager init dir: {}", gameDir.string());

    const std::filesystem::path mainPakPath = gameDir / "FactoryGame/Content/Paks/FactoryGame-Windows.pak";
    const std::filesystem::path mainUtocPath = gameDir / "FactoryGame/Content/Paks/FactoryGame-Windows.utoc";
    const std::filesystem::path globalUtocPath = gameDir / "FactoryGame/Content/Paks/global.utoc";
    const std::filesystem::path modsDir = gameDir / "FactoryGame/Mods";

    if (!std::filesystem::is_regular_file(mainPakPath)) {
        throw std::runtime_error("Main pak file not found!");
    }
    pakFiles_.push_back(std::make_shared<PakFile>(mainPakPath));
    cacheLatestPakNames();

    if (!std::filesystem::is_regular_file(mainUtocPath)) {
        throw std::runtime_error("Main utoc file not found!");
    }
    ioStoreFiles_.push_back(std::make_shared<IoStoreFile>(mainUtocPath));
    ioStoreFiles_.push_back(std::make_shared<IoStoreFile>(globalUtocPath));
    // TODO cache names

    // Search for Mod pak/utoc files.
    if (!std::filesystem::is_directory(modsDir)) {
        // No 'Mods' directory, nothing to do.
        return;
    }

    spdlog::info("Search for pak/utoc files in Mods directory.");
    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(modsDir,
             std::filesystem::directory_options::skip_permission_denied)) {
        if (!std::filesystem::is_regular_file(dirEntry.path())) {
            continue;
        }

        const bool is_pak = dirEntry.path().extension() == ".pak";
        const bool is_utoc = dirEntry.path().extension() == ".utoc";

        // Ignore non pak/utoc files
        if (!is_pak && !is_utoc) {
            continue;
        }

        // Check if dirname matches "<ModName>/Content/Paks/Windows", if we find pak/utoc files with a different
        // structure, the assumptions made on fixing path names are probably not matching.
        const auto filePath = std::filesystem::canonical(dirEntry.path());
        spdlog::info("Found pak/utoc file: {}", filePath.string());
        std::filesystem::path relativePath = std::filesystem::relative(filePath, modsDir);
        const auto pathSegments = splitPathName(relativePath);
        if (pathSegments.size() < 4 || pathSegments[1] != "Content" || pathSegments[2] != "Paks" ||
            pathSegments[3] != "Windows") {
            spdlog::warn("Invalid mods pak/utoc path!");
            continue;
        }

        if (is_pak) {
            std::shared_ptr<PakFile> pak;
            try {
                pak = std::make_shared<PakFile>(filePath);
            } catch (const std::exception& ex) {
                spdlog::error("Error reading pak file: {}", ex.what());
            }
            if (pak != nullptr) {
                pakFiles_.push_back(std::move(pak));
                cacheLatestPakNames(pathSegments[0]);
            }
        } else if (is_utoc) {
            std::shared_ptr<IoStoreFile> ioStore;
            try {
                ioStore = std::make_shared<IoStoreFile>(filePath);
            } catch (const std::exception& ex) {
                spdlog::error("Error reading utoc file: {}", ex.what());
            }
            if (ioStore != nullptr) {
                ioStoreFiles_.push_back(std::move(ioStore));
                // TODO cache names
            }
        }
    }
}

void SatisfactorySave::PakManager::cacheLatestPakNames(const std::optional<std::string>& modPrefix) {
    const std::size_t pakIdx = pakFiles_.size() - 1;
    const auto pakFilenames = pakFiles_.back()->getAllAssetFilenames();
    for (const auto& filename : pakFilenames) {
        // Fix package filenames names to match class paths.
        // https://github.com/EpicGames/UnrealEngine/blob/4.26.2-release/Engine/Source/Runtime/CoreUObject/Private/Misc/PackageName.cpp#L332-L401
        //
        // "Engine/Content/" => "Engine/"
        // "FactoryGame/Content/" => "Game/"
        // "<Modname>/Content/" => "<Modname>/"
        // There are more rules (see Unreal source), but they are not observed in the used pak files so far.
        //
        // Further we to add <ModName>/ prefix here.
        std::string path = filename;
        if (!modPrefix.has_value()) {
            if (filename.starts_with("Engine/Content/")) {
                path = "Engine/" + filename.substr(15);
            } else if (filename.starts_with("FactoryGame/Content/")) {
                path = "Game/" + filename.substr(20);
            }
        } else {
            if (filename.starts_with("Content/")) {
                path = filename.substr(8);
            }
            bool startsWithSlash = !path.empty() && path[0] == '/';
            if (startsWithSlash) {
                path = modPrefix.value() + path;
            } else {
                path = modPrefix.value() + "/" + path;
            }
        }

        if (!packageNames_.insert(std::make_pair(path, std::make_pair(pakIdx, filename))).second) {
            spdlog::error("Package name is not unique: {}, PakIdx: {}, PakFilename: {}", path, pakIdx, filename);
            throw std::runtime_error("Package name is not unique: " + path);
        }
    }
}

std::vector<std::string> SatisfactorySave::PakManager::getAllAssetFilenames() const {
    std::vector<std::string> filenames;
    filenames.reserve(packageNames_.size());
    for (const auto& entry : packageNames_) {
        filenames.emplace_back(entry.first);
    }
    return filenames;
}
