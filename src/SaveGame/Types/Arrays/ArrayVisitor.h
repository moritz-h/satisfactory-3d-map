#ifndef SATISFACTORY3DMAP_ARRAYVISITOR_H
#define SATISFACTORY3DMAP_ARRAYVISITOR_H

#include "ByteArray.h"
#include "EnumArray.h"
#include "IntArray.h"
#include "ObjectArray.h"
#include "StructArray.h"

namespace Satisfactory3DMap {
    class ArrayVisitor {
    public:
        virtual void visit(ByteArray& p) = 0;
        virtual void visit(EnumArray& p) = 0;
        virtual void visit(IntArray& p) = 0;
        virtual void visit(ObjectArray& p) = 0;
        virtual void visit(StructArray& p) = 0;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_ARRAYVISITOR_H
