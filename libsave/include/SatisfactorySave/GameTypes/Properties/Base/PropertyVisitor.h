#pragma once

#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class ArrayProperty;
    class BoolProperty;
    class ByteProperty;
    class DoubleProperty;
    class EnumProperty;
    class FloatProperty;
    class Int64Property;
    class Int8Property;
    class IntProperty;
    class MapProperty;
    class MulticastSparseDelegateProperty;
    class NameProperty;
    class ObjectProperty;
    class SetProperty;
    class SoftObjectProperty;
    class StrProperty;
    class StructProperty;
    class TextProperty;
    class UInt32Property;
    class UInt64Property;
    class UnknownProperty;

    class SATISFACTORYSAVE_API PropertyVisitor {
    public:
        virtual ~PropertyVisitor() = default;

        virtual void visit(ArrayProperty& p) = 0;
        virtual void visit(BoolProperty& p) = 0;
        virtual void visit(ByteProperty& p) = 0;
        virtual void visit(DoubleProperty& p) = 0;
        virtual void visit(EnumProperty& p) = 0;
        virtual void visit(FloatProperty& p) = 0;
        virtual void visit(Int64Property& p) = 0;
        virtual void visit(Int8Property& p) = 0;
        virtual void visit(IntProperty& p) = 0;
        virtual void visit(MapProperty& p) = 0;
        virtual void visit(MulticastSparseDelegateProperty& p) = 0;
        virtual void visit(NameProperty& p) = 0;
        virtual void visit(ObjectProperty& p) = 0;
        virtual void visit(SetProperty& p) = 0;
        virtual void visit(SoftObjectProperty& p) = 0;
        virtual void visit(StrProperty& p) = 0;
        virtual void visit(StructProperty& p) = 0;
        virtual void visit(TextProperty& p) = 0;
        virtual void visit(UInt32Property& p) = 0;
        virtual void visit(UInt64Property& p) = 0;
        virtual void visit(UnknownProperty& p) = 0;
    };
} // namespace SatisfactorySave
