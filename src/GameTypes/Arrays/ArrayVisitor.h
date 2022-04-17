#pragma once

#include "ByteArray.h"
#include "EnumArray.h"
#include "IntArray.h"
#include "ObjectArray.h"
#include "StructArray.h"

namespace Satisfactory3DMap {
    class ArrayVisitor {
    public:
        virtual void visit(ByteArray& a) = 0;
        virtual void visit(EnumArray& a) = 0;
        virtual void visit(IntArray& a) = 0;
        virtual void visit(ObjectArray& a) = 0;
        virtual void visit(StructArray& a) = 0;
    };
} // namespace Satisfactory3DMap
