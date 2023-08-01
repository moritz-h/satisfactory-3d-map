#include "EnumSetting.h"

#include "SettingVisitor.h"

void Satisfactory3DMap::EnumSettingBase::accept(Satisfactory3DMap::SettingVisitor& v) {
    v.visit(*this);
}

void Satisfactory3DMap::EnumSettingBase::serializeFromJson(const nlohmann::json& j) {
    std::size_t idx = 0;
    j.get_to(idx);
    if (idx < names_.size()) {
        currentIdx_ = idx;
    } else {
        throw std::runtime_error("Invalid serialization index!");
    }
}

void Satisfactory3DMap::EnumSettingBase::serializeToJson(nlohmann::json& j) {
    j = currentIdx_;
}
