#include "StringSetting.h"

#include "SettingVisitor.h"

void Satisfactory3DMap::StringSetting::accept(Satisfactory3DMap::SettingVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::StringSetting::serializeFromJson(const nlohmann::json& j) {
    j.get_to(value_);
}

void Satisfactory3DMap::StringSetting::serializeToJson(nlohmann::json& j) {
    j = value_;
}
