#pragma once

#include <memory>
#include <string>

#include <nlohmann/json.hpp>

namespace Satisfactory3DMap {
    class SettingVisitor;

    class Setting {
    public:
        explicit Setting(std::string name) : name_(std::move(name)) {}

        virtual void accept(SettingVisitor& v) = 0;

        [[nodiscard]] const std::string& name() const {
            return name_;
        }

    protected:
        friend class Configuration;

        void registerConfig(std::weak_ptr<Configuration> config) {
            config_ = std::move(config);
        }

        void update();

        virtual void serializeFromJson(const nlohmann::json& j) = 0;
        virtual void serializeToJson(nlohmann::json& j) = 0;

        std::string name_;
        std::weak_ptr<Configuration> config_;
    };
} // namespace Satisfactory3DMap
