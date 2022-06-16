#pragma once

#include "StructSet.h"

namespace Satisfactory3DMap {
    class SetVisitor {
    public:
        virtual void visit(StructSet& a) = 0;
    };
} // namespace Satisfactory3DMap
