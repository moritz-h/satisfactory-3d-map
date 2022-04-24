#include "Configuration.h"

#include <fstream>
#include <iomanip>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "Utils/FilesystemUtil.h"

Satisfactory3DMap::Configuration::Configuration() {
    auto cfgFile = getConfigFile();
    if (!std::filesystem::exists(cfgFile)) {
        spdlog::info("No config file!");
        return;
    }
    cfgFile = std::filesystem::canonical(cfgFile);
    if (!std::filesystem::is_regular_file(cfgFile)) {
        spdlog::error("Config file path is not a file: {}", cfgFile.string());
        return;
    }
    spdlog::info("Read config file: {}", cfgFile.string());
    try {
        std::ifstream file(cfgFile);
        nlohmann::json j;
        file >> j;
        j.at("gameDirectory").get_to(gameDirectory_);
        if (!std::filesystem::is_directory(gameDirectory_)) {
            gameDirectory_.clear();
        }
    } catch (const std::exception& ex) {
        spdlog::error("Error reading config: {}", ex.what());
    }
}

void Satisfactory3DMap::Configuration::saveOnDisk() const {
    try {
        nlohmann::json j{
            {"gameDirectory", gameDirectory_.string()},
        };
        std::ofstream file(getConfigFile());
        file << std::setw(4) << j << std::endl;
    } catch (const std::exception& ex) {
        spdlog::error("Error writing config: {}", ex.what());
    }
}
