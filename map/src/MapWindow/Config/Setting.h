#pragma once

#include <memory>
#include <string>

#include <nlohmann/json.hpp>

namespace Satisfactory3DMap {
    class Configuration;
    class SettingVisitor;

    class Setting {
    public:
        explicit Setting(std::string name) : name_(std::move(name)), hidden_(false) {}
        virtual ~Setting() = default;

        virtual void accept(SettingVisitor& v) = 0;

        [[nodiscard]] const std::string& name() const {
            return name_;
        }

        [[nodiscard]] inline bool isHidden() const {
            return hidden_;
        }

        inline void hide() {
            hidden_ = true;
        }

        inline void show() {
            hidden_ = false;
        }

        virtual void resetDefault() = 0;

    protected:
        friend class Configuration;

        void registerConfig(std::weak_ptr<Configuration> config) {
            config_ = std::move(config);
        }

        void update();

        virtual void serializeFromJson(const nlohmann::json& j) = 0;
        virtual void serializeToJson(nlohmann::json& j) = 0;

        std::string name_;
        bool hidden_;
        std::weak_ptr<Configuration> config_;
    };
} // namespace Satisfactory3DMap
