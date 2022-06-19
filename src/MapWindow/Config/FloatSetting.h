#pragma once

#include <memory>

#include "Setting.h"

namespace Satisfactory3DMap {
    class FloatSetting : public Setting {
    public:
        static std::shared_ptr<FloatSetting> create(std::string name, float init) {
            return std::make_shared<FloatSetting>(std::move(name), init);
        }

        explicit FloatSetting(std::string name, float init) : Setting(std::move(name)), value_(init) {}

        void accept(SettingVisitor& v) override;

        [[nodiscard]] float getVal() const {
            return value_;
        }

        void setVal(float val) {
            value_ = val;
        }

    protected:
        float value_;
    };
} // namespace Satisfactory3DMap
