#pragma once

#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class BoolArray;
    class ByteArray;
    class EnumArray;
    class FloatArray;
    class Int64Array;
    class Int8Array;
    class IntArray;
    class InterfaceArray;
    class NameArray;
    class ObjectArray;
    class SoftObjectArray;
    class StrArray;
    class StructArray;

    class SATISFACTORYSAVE_API ArrayVisitor {
    public:
        virtual ~ArrayVisitor() = default;

        virtual void visit(BoolArray& a) = 0;
        virtual void visit(ByteArray& a) = 0;
        virtual void visit(EnumArray& a) = 0;
        virtual void visit(FloatArray& a) = 0;
        virtual void visit(Int64Array& a) = 0;
        virtual void visit(Int8Array& a) = 0;
        virtual void visit(IntArray& a) = 0;
        virtual void visit(InterfaceArray& a) = 0;
        virtual void visit(NameArray& a) = 0;
        virtual void visit(ObjectArray& a) = 0;
        virtual void visit(SoftObjectArray& a) = 0;
        virtual void visit(StrArray& a) = 0;
        virtual void visit(StructArray& a) = 0;
    };
} // namespace SatisfactorySave
