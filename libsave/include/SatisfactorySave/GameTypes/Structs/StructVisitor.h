#pragma once

#include "BoxStruct.h"
#include "ColorStruct.h"
#include "FluidBoxStruct.h"
#include "GuidStruct.h"
#include "IntPointStruct.h"
#include "IntVectorStruct.h"
#include "InventoryItemStruct.h"
#include "LinearColorStruct.h"
#include "MaterialInput.h"
#include "Mods/LBBalancerIndexingStruct.h"
#include "PropertyStruct.h"
#include "QuatStruct.h"
#include "RailroadTrackPositionStruct.h"
#include "RotatorStruct.h"
#include "SoftClassPathStruct.h"
#include "Vector2DStruct.h"
#include "VectorStruct.h"

namespace SatisfactorySave {

    class StructVisitor {
    public:
        virtual void visit(BoxStruct& s) = 0;
        virtual void visit(ColorStruct& s) = 0;
        virtual void visit(FluidBoxStruct& s) = 0;
        virtual void visit(GuidStruct& s) = 0;
        virtual void visit(IntPointStruct& s) = 0;
        virtual void visit(IntVectorStruct& s) = 0;
        virtual void visit(InventoryItemStruct& s) = 0;
        virtual void visit(LBBalancerIndexingStruct& s) = 0;
        virtual void visit(LinearColorStruct& s) = 0;
        virtual void visit(PropertyStruct& s) = 0;
        virtual void visit(QuatStruct& s) = 0;
        virtual void visit(RailroadTrackPositionStruct& s) = 0;
        virtual void visit(RotatorStruct& s) = 0;
        virtual void visit(ScalarMaterialInputStruct& s) = 0;
        virtual void visit(SoftClassPathStruct& s) = 0;
        virtual void visit(VectorMaterialInputStruct& s) = 0;
        virtual void visit(Vector2DStruct& s) = 0;
        virtual void visit(VectorStruct& s) = 0;
    };
} // namespace SatisfactorySave
