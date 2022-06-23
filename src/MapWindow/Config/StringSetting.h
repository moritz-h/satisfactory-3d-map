#pragma once

#include <memory>
#include <string>

#include "Setting.h"

namespace Satisfactory3DMap {
    class StringSetting : public Setting {
    public:
        static std::shared_ptr<StringSetting> create(std::string name, std::string init = "") {
            return std::make_shared<StringSetting>(std::move(name), std::move(init));
        }

        explicit StringSetting(std::string name, std::string init = "")
            : Setting(std::move(name)),
              defaultValue_(std::move(init)),
              value_(defaultValue_) {}

        void accept(SettingVisitor& v) override;

        [[nodiscard]] inline const std::string& getVal() const {
            return value_;
        }

        void setVal(const std::string& val) {
            if (value_ != val) {
                value_ = val;
                update();
            }
        }

        void resetDefault() override {
            setVal(defaultValue_);
        }

    protected:
        void serializeFromJson(const nlohmann::json& j) override;
        void serializeToJson(nlohmann::json& j) override;

        std::string defaultValue_;
        std::string value_;
    };
} // namespace Satisfactory3DMap
