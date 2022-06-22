#pragma once

#include <memory>

#include "Setting.h"

namespace Satisfactory3DMap {
    class FloatSetting : public Setting {
    public:
        static std::shared_ptr<FloatSetting> create(std::string name, float init = 0.0f) {
            return std::make_shared<FloatSetting>(std::move(name), init);
        }

        explicit FloatSetting(std::string name, float init = 0.0f) : Setting(std::move(name)), value_(init) {}

        void accept(SettingVisitor& v) override;

        [[nodiscard]] inline float getVal() const {
            return value_;
        }

        void setVal(float val) {
            if (value_ != val) {
                value_ = val;
                update();
            }
        }

    protected:
        void serializeFromJson(const nlohmann::json& j) override;
        void serializeToJson(nlohmann::json& j) override;

        float value_;
    };
} // namespace Satisfactory3DMap
