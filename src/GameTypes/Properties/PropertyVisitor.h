#pragma once

#include "ArrayProperty.h"
#include "BoolProperty.h"
#include "ByteProperty.h"
#include "EnumProperty.h"
#include "FloatProperty.h"
#include "Int64Property.h"
#include "Int8Property.h"
#include "IntProperty.h"
#include "MapProperty.h"
#include "NameProperty.h"
#include "ObjectProperty.h"
#include "StrProperty.h"
#include "StructProperty.h"
#include "TextProperty.h"

namespace Satisfactory3DMap {
    class PropertyVisitor {
    public:
        virtual void visit(ArrayProperty& p) = 0;
        virtual void visit(BoolProperty& p) = 0;
        virtual void visit(ByteProperty& p) = 0;
        virtual void visit(EnumProperty& p) = 0;
        virtual void visit(FloatProperty& p) = 0;
        virtual void visit(Int64Property& p) = 0;
        virtual void visit(Int8Property& p) = 0;
        virtual void visit(IntProperty& p) = 0;
        virtual void visit(MapProperty& p) = 0;
        virtual void visit(NameProperty& p) = 0;
        virtual void visit(ObjectProperty& p) = 0;
        virtual void visit(StrProperty& p) = 0;
        virtual void visit(StructProperty& p) = 0;
        virtual void visit(TextProperty& p) = 0;
    };
} // namespace Satisfactory3DMap
