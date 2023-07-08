#include "FloatSetting.h"

#include "SettingVisitor.h"

void Satisfactory3DMap::FloatSetting::accept(Satisfactory3DMap::SettingVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::FloatSetting::serializeFromJson(const nlohmann::json& j) {
    j.get_to(value_);
}

void Satisfactory3DMap::FloatSetting::serializeToJson(nlohmann::json& j) {
    j = value_;
}
