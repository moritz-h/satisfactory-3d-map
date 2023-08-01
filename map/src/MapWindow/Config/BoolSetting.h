#pragma once

#include <memory>

#include "Setting.h"

namespace Satisfactory3DMap {
    class BoolSetting : public Setting {
    public:
        static std::shared_ptr<BoolSetting> create(std::string name, bool init = false) {
            return std::make_shared<BoolSetting>(std::move(name), init);
        }

        explicit BoolSetting(std::string name, bool init = false)
            : Setting(std::move(name)),
              defaultValue_(init),
              value_(init) {}

        void accept(SettingVisitor& v) override;

        [[nodiscard]] inline bool getVal() const {
            return value_;
        }

        void setVal(bool val) {
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

        bool defaultValue_;
        bool value_;
    };
} // namespace Satisfactory3DMap
