#pragma once

#include "ByteMapTypeList.h"
#include "EnumMapTypeList.h"
#include "IntMapTypeList.h"
#include "ObjectMapTypeList.h"
#include "StructMapTypeList.h"

namespace Satisfactory3DMap {
    class MapTypeListVisitor {
    public:
        virtual void visit(ByteMapTypeList& m) = 0;
        virtual void visit(EnumMapTypeList& m) = 0;
        virtual void visit(IntMapTypeList& m) = 0;
        virtual void visit(ObjectMapTypeList& m) = 0;
        virtual void visit(StructMapTypeList& m) = 0;
    };
} // namespace Satisfactory3DMap
