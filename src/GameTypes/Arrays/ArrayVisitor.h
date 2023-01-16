#pragma once

#include "BoolArray.h"
#include "ByteArray.h"
#include "EnumArray.h"
#include "FloatArray.h"
#include "Int64Array.h"
#include "IntArray.h"
#include "NameArray.h"
#include "ObjectArray.h"
#include "SoftObjectArray.h"
#include "StrArray.h"
#include "StructArray.h"

namespace Satisfactory3DMap {
    class ArrayVisitor {
    public:
        virtual void visit(BoolArray& a) = 0;
        virtual void visit(ByteArray& a) = 0;
        virtual void visit(EnumArray& a) = 0;
        virtual void visit(FloatArray& a) = 0;
        virtual void visit(Int64Array& a) = 0;
        virtual void visit(IntArray& a) = 0;
        virtual void visit(NameArray& a) = 0;
        virtual void visit(ObjectArray& a) = 0;
        virtual void visit(SoftObjectArray& a) = 0;
        virtual void visit(StrArray& a) = 0;
        virtual void visit(StructArray& a) = 0;
    };
} // namespace Satisfactory3DMap
