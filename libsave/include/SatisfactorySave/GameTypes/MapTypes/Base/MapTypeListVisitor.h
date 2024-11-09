#pragma once

#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class ByteMapTypeList;
    class EnumMapTypeList;
    class FloatMapTypeList;
    class Int64MapTypeList;
    class IntMapTypeList;
    class NameMapTypeList;
    class ObjectMapTypeList;
    class StructMapTypeList;

    class SATISFACTORYSAVE_API MapTypeListVisitor {
    public:
        virtual ~MapTypeListVisitor() = default;

        virtual void visit(ByteMapTypeList& m) = 0;
        virtual void visit(EnumMapTypeList& m) = 0;
        virtual void visit(FloatMapTypeList& m) = 0;
        virtual void visit(Int64MapTypeList& m) = 0;
        virtual void visit(IntMapTypeList& m) = 0;
        virtual void visit(NameMapTypeList& m) = 0;
        virtual void visit(ObjectMapTypeList& m) = 0;
        virtual void visit(StructMapTypeList& m) = 0;
    };
} // namespace SatisfactorySave
