#pragma once

#include <string>

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
        std::string name_;
    };
} // namespace Satisfactory3DMap
