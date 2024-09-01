#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "Setting.h"

namespace Satisfactory3DMap {

    class Configuration : public std::enable_shared_from_this<Configuration> {
    public:
        static std::shared_ptr<Configuration> create() {
            return std::shared_ptr<Configuration>(new Configuration());
        }

        ~Configuration() = default;

        void registerSetting(std::shared_ptr<Setting> setting);

        /*
         * Call once after all initialization is done! All save requests are blocked by this to avoid writing an
         * incomplete configuration before all settings are registered.
         */
        void registerDone();

        void requestSave();

    protected:
        Configuration();

        void saveOnDisk();

        friend class SettingsWindow;

        [[nodiscard]] const std::vector<std::shared_ptr<Setting>>& getSettings() const {
            return settings_;
        }

        nlohmann::json json_;
        std::vector<std::shared_ptr<Setting>> settings_;
        bool registerDone_ = false;
        bool requestSave_ = false;
    };
} // namespace Satisfactory3DMap
