#ifndef SATISFACTORY3DMAP_STRUCTVISITOR_H
#define SATISFACTORY3DMAP_STRUCTVISITOR_H

#include "BoxStruct.h"
#include "ColorStruct.h"
#include "FluidBoxStruct.h"
#include "GuidStruct.h"
#include "IntPointStruct.h"
#include "InventoryItemStruct.h"
#include "LinearColorStruct.h"
#include "PropertyStruct.h"
#include "QuatStruct.h"
#include "RailroadTrackPositionStruct.h"
#include "RotatorStruct.h"
#include "VectorStruct.h"

namespace Satisfactory3DMap {
    class StructVisitor {
    public:
        virtual void visit(BoxStruct& s) = 0;
        virtual void visit(ColorStruct& s) = 0;
        virtual void visit(FluidBoxStruct& s) = 0;
        virtual void visit(GuidStruct& s) = 0;
        virtual void visit(IntPointStruct& s) = 0;
        virtual void visit(InventoryItemStruct& s) = 0;
        virtual void visit(LinearColorStruct& s) = 0;
        virtual void visit(PropertyStruct& s) = 0;
        virtual void visit(QuatStruct& s) = 0;
        virtual void visit(RailroadTrackPositionStruct& s) = 0;
        virtual void visit(RotatorStruct& s) = 0;
        virtual void visit(VectorStruct& s) = 0;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCTVISITOR_H
