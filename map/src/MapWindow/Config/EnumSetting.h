#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Setting.h"

namespace Satisfactory3DMap {
    class EnumSettingBase : public Setting {
    public:
        EnumSettingBase(std::string name, std::vector<const char*> names, std::size_t init = 0)
            : Setting(std::move(name)),
              names_(std::move(names)) {
            if (names_.empty() || init >= names_.size()) {
                throw std::invalid_argument("Bad enum configuration!");
            }
            defaultIdx_ = init;
            currentIdx_ = init;
        }

        void accept(SettingVisitor& v) override;

        [[nodiscard]] inline const std::vector<const char*>& names() const {
            return names_;
        }

        [[nodiscard]] inline std::size_t getIdx() const {
            return currentIdx_;
        }

        void setIdx(std::size_t idx) {
            if (idx >= names_.size()) {
                throw std::invalid_argument("Invalid index!");
            }
            if (currentIdx_ != idx) {
                currentIdx_ = idx;
                update();
            }
        }

        void resetDefault() override {
            setIdx(defaultIdx_);
        }

    protected:
        void serializeFromJson(const nlohmann::json& j) override;
        void serializeToJson(nlohmann::json& j) override;

        std::vector<const char*> names_;
        std::size_t defaultIdx_;
        std::size_t currentIdx_;
    };

    template<typename T>
    class EnumSetting : public EnumSettingBase {
    public:
        static std::shared_ptr<EnumSetting<T>> create(std::string name, std::vector<const char*> names,
            std::vector<T> values, std::size_t init = 0) {
            return std::make_shared<EnumSetting<T>>(std::move(name), std::move(names), std::move(values), init);
        }

        EnumSetting(std::string name, std::vector<const char*> names, std::vector<T> values, std::size_t init = 0)
            : EnumSettingBase(std::move(name), std::move(names), init),
              values_(std::move(values)) {
            if (names_.size() != values_.size()) {
                throw std::invalid_argument("Bad enum configuration!");
            }
        }

        [[nodiscard]] inline const T& getVal() const {
            return values_[currentIdx_];
        }

        void setVal(T val) {
            auto it = std::find(values_.begin(), values_.end(), val);
            if (it != values_.end()) {
                setIdx(std::distance(values_.begin(), it));
            } else {
                throw std::runtime_error("Invalid value!");
            }
        }

    protected:
        std::vector<T> values_;
    };
} // namespace Satisfactory3DMap
