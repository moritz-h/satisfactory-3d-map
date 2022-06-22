#include "BoolSetting.h"

#include "SettingVisitor.h"

void Satisfactory3DMap::BoolSetting::accept(Satisfactory3DMap::SettingVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::BoolSetting::serializeFromJson(const nlohmann::json& j) {
    j.get_to(value_);
}

void Satisfactory3DMap::BoolSetting::serializeToJson(nlohmann::json& j) {
    j = value_;
}
