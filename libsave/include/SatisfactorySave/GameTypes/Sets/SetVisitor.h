#pragma once

#include "StructSet.h"
#include "UInt32Set.h"

namespace SatisfactorySave {

    class SetVisitor {
    public:
        virtual void visit(StructSet& a) = 0;
        virtual void visit(UInt32Set& a) = 0;
    };
} // namespace SatisfactorySave
