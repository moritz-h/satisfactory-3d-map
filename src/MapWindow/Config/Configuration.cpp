#include "Configuration.h"

#include <fstream>
#include <iomanip>

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
        file >> json_;
        if (json_.contains("gameDirectory")) {
            json_.at("gameDirectory").get_to(gameDirectory_);
        }
        if (!std::filesystem::is_directory(gameDirectory_)) {
            gameDirectory_.clear();
        }
    } catch (const std::exception& ex) {
        spdlog::error("Error reading config: {}", ex.what());
    }
}

void Satisfactory3DMap::Configuration::registerSetting(std::shared_ptr<Setting> setting) {
    setting->registerConfig(weak_from_this());
    try {
        if (json_.contains(setting->name())) {
            setting->serializeFromJson(json_.at(setting->name()));
        }
    } catch (const std::exception& ex) {
        spdlog::error("Error serializing setting {}: {}", setting->name(), ex.what());
    }
    settings_.emplace_back(std::move(setting));
}

void Satisfactory3DMap::Configuration::requestSave() {
    // TODO cache save request and only write every x seconds to disk.
    saveOnDisk();
}

void Satisfactory3DMap::Configuration::saveOnDisk() const {
    try {
        nlohmann::json j{
            {"gameDirectory", gameDirectory_.string()},
        };
        for (const auto& s : settings_) {
            nlohmann::json serialized;
            s->serializeToJson(serialized);
            j[s->name()] = serialized;
        }
        std::ofstream file(getConfigFile());
        file << std::setw(4) << j << std::endl;
    } catch (const std::exception& ex) {
        spdlog::error("Error writing config: {}", ex.what());
    }
}
