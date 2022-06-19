#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "Setting.h"

namespace Satisfactory3DMap {

    class Configuration {
    public:
        Configuration();
        ~Configuration() = default;

        void saveOnDisk() const;

        void registerSetting(std::shared_ptr<Setting> setting) {
            settings_.emplace_back(std::move(setting));
        }

        [[nodiscard]] const std::string& getImGuiIni() const {
            return imGuiIni_;
        }

        void setImGuiIni(const std::string& imGuiIni) {
            imGuiIni_ = imGuiIni;
            saveOnDisk();
        }

        [[nodiscard]] const std::filesystem::path& getGameDirectory() const {
            return gameDirectory_;
        }

        void setGameDirectory(const std::filesystem::path& gameDirectory) {
            gameDirectory_ = gameDirectory;
            saveOnDisk();
        }

    protected:
        friend class SettingsWindow;

        [[nodiscard]] const std::vector<std::shared_ptr<Setting>>& getSettings() const {
            return settings_;
        }

        std::vector<std::shared_ptr<Setting>> settings_;

        std::string imGuiIni_;
        std::filesystem::path gameDirectory_;
    };
} // namespace Satisfactory3DMap
