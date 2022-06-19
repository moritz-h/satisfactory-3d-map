#include "FloatSetting.h"

#include "SettingVisitor.h"

void Satisfactory3DMap::FloatSetting::accept(Satisfactory3DMap::SettingVisitor& v) {
    v.visit(*this);
}
