#ifndef SATISFACTORY3DMAP_STRUCTVISITOR_H
#define SATISFACTORY3DMAP_STRUCTVISITOR_H

#include "BoxStruct.h"
#include "ColorStruct.h"
#include "FluidBoxStruct.h"
#include "InventoryItemStruct.h"
#include "LinearColorStruct.h"
#include "PropertyStruct.h"
#include "QuatStruct.h"
#include "RailroadTrackPositionStruct.h"
#include "VectorStruct.h"

namespace Satisfactory3DMap {
    class StructVisitor {
    public:
        virtual void visit(BoxStruct& p) = 0;
        virtual void visit(ColorStruct& p) = 0;
        virtual void visit(FluidBoxStruct& p) = 0;
        virtual void visit(InventoryItemStruct& p) = 0;
        virtual void visit(LinearColorStruct& p) = 0;
        virtual void visit(PropertyStruct& p) = 0;
        virtual void visit(QuatStruct& p) = 0;
        virtual void visit(RailroadTrackPositionStruct& p) = 0;
        virtual void visit(VectorStruct& p) = 0;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_STRUCTVISITOR_H
