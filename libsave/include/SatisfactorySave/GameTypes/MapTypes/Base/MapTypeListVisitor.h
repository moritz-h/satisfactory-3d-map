#pragma once

#include "ByteMapTypeList.h"
#include "EnumMapTypeList.h"
#include "FloatMapTypeList.h"
#include "IntMapTypeList.h"
#include "NameMapTypeList.h"
#include "ObjectMapTypeList.h"
#include "StructMapTypeList.h"

namespace SatisfactorySave {

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
