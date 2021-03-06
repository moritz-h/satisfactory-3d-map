#pragma once

#include "BoxStruct.h"
#include "ColorStruct.h"
#include "FluidBoxStruct.h"
#include "GuidStruct.h"
#include "IntPointStruct.h"
#include "InventoryItemStruct.h"
#include "LinearColorStruct.h"
#include "MaterialInput.h"
#include "PropertyStruct.h"
#include "QuatStruct.h"
#include "RailroadTrackPositionStruct.h"
#include "RotatorStruct.h"
#include "SoftClassPathStruct.h"
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
        virtual void visit(ScalarMaterialInputStruct& s) = 0;
        virtual void visit(SoftClassPathStruct& s) = 0;
        virtual void visit(VectorMaterialInputStruct& s) = 0;
        virtual void visit(VectorStruct& s) = 0;
    };
} // namespace Satisfactory3DMap
