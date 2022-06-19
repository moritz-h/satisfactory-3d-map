#include "BoolSetting.h"

#include "SettingVisitor.h"

void Satisfactory3DMap::BoolSetting::accept(Satisfactory3DMap::SettingVisitor& v) {
    v.visit(*this);
}
