#pragma once

#include <filesystem>
#include <memory>

#include "Setting.h"

namespace Satisfactory3DMap {
    class PathSetting : public Setting {
    public:
        enum class PathType { Directory, File };

        static std::shared_ptr<PathSetting> create(std::string name, PathType type, std::filesystem::path init = "") {
            return std::make_shared<PathSetting>(std::move(name), type, std::move(init));
        }

        PathSetting(std::string name, PathType type, std::filesystem::path init = "")
            : Setting(std::move(name)),
              type_(type),
              value_(std::move(init)) {
            if (!value_.empty() && !validate(value_)) {
                value_.clear();
            }
        }

        void accept(SettingVisitor& v) override;

        [[nodiscard]] inline PathType getType() const {
            return type_;
        }

        [[nodiscard]] inline const std::filesystem::path& getVal() const {
            return value_;
        }

        void setVal(const std::filesystem::path& val) {
            if (value_ != val && validate(val)) {
                value_ = val;
                update();
            }
        }

    protected:
        void serializeFromJson(const nlohmann::json& j) override;
        void serializeToJson(nlohmann::json& j) override;

        bool validate(const std::filesystem::path& path);

        PathType type_;
        std::filesystem::path value_;
    };
} // namespace Satisfactory3DMap
