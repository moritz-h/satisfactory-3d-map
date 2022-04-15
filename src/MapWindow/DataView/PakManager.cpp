#include "PakManager.h"

#include <spdlog/spdlog.h>

Satisfactory3DMap::PakManager::PakManager(const std::filesystem::path& gameDir) {
    spdlog::info("PakManager init dir: {}", gameDir.string());

    // only search for pak files within /FactoryGame.
    std::filesystem::path factoryGameDir = gameDir / "FactoryGame";
    if (!std::filesystem::is_directory(factoryGameDir)) {
        throw std::runtime_error("FactoryGame director missing!");
    }

    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(factoryGameDir,
             std::filesystem::directory_options::skip_permission_denied)) {
        if (dirEntry.path().extension() == ".pak" && std::filesystem::is_regular_file(dirEntry.path())) {
            const auto pakPath = std::filesystem::canonical(dirEntry.path());
            spdlog::info("Found pak file: {}", pakPath.string());
            try {
                auto pak = std::make_shared<PakFile>(pakPath);
                pakFiles_.push_back(std::move(pak));
                pakFileNames_.push_back(std::filesystem::relative(pakPath, gameDir).string());
            } catch (const std::exception& ex) { spdlog::error("Error reading pak file: {}", ex.what()); }
        }
    }
}
