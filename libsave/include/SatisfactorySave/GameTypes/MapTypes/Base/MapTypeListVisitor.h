#pragma once

namespace SatisfactorySave {

    class ByteMapTypeList;
    class EnumMapTypeList;
    class FloatMapTypeList;
    class IntMapTypeList;
    class NameMapTypeList;
    class ObjectMapTypeList;
    class StructMapTypeList;

    class MapTypeListVisitor {
    public:
        virtual void visit(ByteMapTypeList& m) = 0;
        virtual void visit(EnumMapTypeList& m) = 0;
        virtual void visit(FloatMapTypeList& m) = 0;
        virtual void visit(IntMapTypeList& m) = 0;
        virtual void visit(NameMapTypeList& m) = 0;
        virtual void visit(ObjectMapTypeList& m) = 0;
        virtual void visit(StructMapTypeList& m) = 0;
    };
} // namespace SatisfactorySave
