#pragma once

#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class BoxStruct;
    class ClientIdentityInfoStruct;
    class ColorStruct;
    class DateTimeStruct;
    class FluidBoxStruct;
    class GuidStruct;
    class IntPointStruct;
    class IntVectorStruct;
    class InventoryItemStruct;
    class LBBalancerIndexingStruct;
    class LinearColorStruct;
    class PropertyStruct;
    class QuatStruct;
    class RailroadTrackPositionStruct;
    class RotatorStruct;
    class ScalarMaterialInputStruct;
    class SoftClassPathStruct;
    class VectorMaterialInputStruct;
    class Vector2DStruct;
    class Vector4Struct;
    class VectorStruct;

    class SATISFACTORYSAVE_API StructVisitor {
    public:
        virtual ~StructVisitor() = default;

        virtual void visit(BoxStruct& s) = 0;
        virtual void visit(ClientIdentityInfoStruct& s) = 0;
        virtual void visit(ColorStruct& s) = 0;
        virtual void visit(DateTimeStruct& s) = 0;
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
        virtual void visit(Vector4Struct& s) = 0;
        virtual void visit(VectorStruct& s) = 0;
    };
} // namespace SatisfactorySave
