#pragma once

#include <memory>

#include "Setting.h"

namespace Satisfactory3DMap {
    class BoolSetting : public Setting {
    public:
        static std::shared_ptr<BoolSetting> create(std::string name, bool init) {
            return std::make_shared<BoolSetting>(std::move(name), init);
        }

        explicit BoolSetting(std::string name, bool init) : Setting(std::move(name)), value_(init) {}

        void accept(SettingVisitor& v) override;

        [[nodiscard]] bool getVal() const {
            return value_;
        }

        void setVal(bool val) {
            value_ = val;
        }

    protected:
        bool value_;
    };
} // namespace Satisfactory3DMap
