#include "EnumSetting.h"

#include "SettingVisitor.h"

void Satisfactory3DMap::EnumSettingBase::accept(Satisfactory3DMap::SettingVisitor& v) {
    v.visit(*this);
}
