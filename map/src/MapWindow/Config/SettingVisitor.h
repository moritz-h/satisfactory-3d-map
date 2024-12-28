#pragma once

#include "BoolSetting.h"
#include "EnumSetting.h"
#include "FloatSetting.h"
#include "PathSetting.h"
#include "StringSetting.h"

namespace Satisfactory3DMap {
    class SettingVisitor {
    public:
        virtual ~SettingVisitor() = default;

        virtual void visit(BoolSetting& s) = 0;
        virtual void visit(EnumSettingBase& s) = 0;
        virtual void visit(FloatSetting& s) = 0;
        virtual void visit(PathSetting& s) = 0;
        virtual void visit(StringSetting& s) = 0;
    };
} // namespace Satisfactory3DMap
