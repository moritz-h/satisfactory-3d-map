#pragma once

#include "SatisfactorySave/GameTypes/Arrays/Base/ArrayAll.h"
#include "SatisfactorySave/GameTypes/Arrays/Base/ArrayVisitor.h"
#include "SatisfactorySave/GameTypes/MapTypes/Base/MapTypeListAll.h"
#include "SatisfactorySave/GameTypes/MapTypes/Base/MapTypeListVisitor.h"
#include "SatisfactorySave/GameTypes/Properties/Base/PropertyAll.h"
#include "SatisfactorySave/GameTypes/Properties/Base/PropertyVisitor.h"
#include "SatisfactorySave/GameTypes/Sets/Base/SetAll.h"
#include "SatisfactorySave/GameTypes/Sets/Base/SetVisitor.h"
#include "SatisfactorySave/GameTypes/Structs/Base/StructAll.h"
#include "SatisfactorySave/GameTypes/Structs/Base/StructVisitor.h"

#include "CommonUI.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap::UI {

    class PropertyEditor : public s::PropertyVisitor {
    public:
        explicit PropertyEditor(const EventContext& ctx = {}) : ctx_(ctx), changed_(false) {};

        [[nodiscard]] bool changed() const {
            return changed_;
        }

        void visit(s::ArrayProperty& p) override;
        void visit(s::BoolProperty& p) override;
        void visit(s::ByteProperty& p) override;
        void visit(s::DoubleProperty& p) override;
        void visit(s::EnumProperty& p) override;
        void visit(s::FloatProperty& p) override;
        void visit(s::Int64Property& p) override;
        void visit(s::Int8Property& p) override;
        void visit(s::IntProperty& p) override;
        void visit(s::MapProperty& p) override;
        void visit(s::MulticastSparseDelegateProperty& p) override;
        void visit(s::NameProperty& p) override;
        void visit(s::ObjectProperty& p) override;
        void visit(s::SetProperty& p) override;
        void visit(s::SoftObjectProperty& p) override;
        void visit(s::StrProperty& p) override;
        void visit(s::StructProperty& p) override;
        void visit(s::TextProperty& p) override;
        void visit(s::UInt32Property& p) override;
        void visit(s::UInt64Property& p) override;
        void visit(s::UnknownProperty& p) override;

    protected:
        const EventContext& ctx_;
        bool changed_;

        class ArrayEditor : public s::ArrayVisitor {
        protected:
            PropertyEditor& parent_;

        public:
            explicit ArrayEditor(PropertyEditor& parent) : parent_(parent) {}

            void visit(s::BoolArray& a) override;
            void visit(s::ByteArray& a) override;
            void visit(s::EnumArray& a) override;
            void visit(s::FloatArray& a) override;
            void visit(s::Int64Array& a) override;
            void visit(s::Int8Array& a) override;
            void visit(s::IntArray& a) override;
            void visit(s::InterfaceArray& a) override;
            void visit(s::NameArray& a) override;
            void visit(s::ObjectArray& a) override;
            void visit(s::SoftObjectArray& a) override;
            void visit(s::StrArray& a) override;
            void visit(s::StructArray& a) override;
        };

        class MapTypeEditor : public s::MapTypeListVisitor {
        protected:
            PropertyEditor& parent_;
            std::size_t idx_;
            bool isKey_;

            static constexpr const char* keyLabel_ = "Key";
            static constexpr const char* valueLabel_ = "Value";

        public:
            MapTypeEditor(PropertyEditor& parent, std::size_t idx, bool isKey)
                : parent_(parent),
                  idx_(idx),
                  isKey_(isKey) {}

            [[nodiscard]] const char* label() const {
                return isKey_ ? keyLabel_ : valueLabel_;
            }

            void visit(s::ByteMapTypeList& m) override;
            void visit(s::EnumMapTypeList& m) override;
            void visit(s::FloatMapTypeList& m) override;
            void visit(s::Int64MapTypeList& m) override;
            void visit(s::IntMapTypeList& m) override;
            void visit(s::NameMapTypeList& m) override;
            void visit(s::ObjectMapTypeList& m) override;
            void visit(s::StructMapTypeList& m) override;
        };

        class SetEditor : public s::SetVisitor {
        protected:
            PropertyEditor& parent_;

        public:
            explicit SetEditor(PropertyEditor& parent) : parent_(parent) {}

            void visit(s::ObjectSet& s) override;
            void visit(s::StructSet& s) override;
            void visit(s::UInt32Set& s) override;
        };

        class StructEditor : public s::StructVisitor {
        protected:
            PropertyEditor& parent_;

        public:
            explicit StructEditor(PropertyEditor& parent) : parent_(parent) {}

            void visit(s::BoxStruct& s) override;
            void visit(s::ClientIdentityInfoStruct& s) override;
            void visit(s::ColorStruct& s) override;
            void visit(s::FluidBoxStruct& s) override;
            void visit(s::GuidStruct& s) override;
            void visit(s::IntPointStruct& s) override;
            void visit(s::IntVectorStruct& s) override;
            void visit(s::InventoryItemStruct& s) override;
            void visit(s::LBBalancerIndexingStruct& s) override;
            void visit(s::LinearColorStruct& s) override;
            void visit(s::PropertyStruct& s) override;
            void visit(s::QuatStruct& s) override;
            void visit(s::RailroadTrackPositionStruct& s) override;
            void visit(s::RotatorStruct& s) override;
            void visit(s::ScalarMaterialInputStruct& s) override;
            void visit(s::SoftClassPathStruct& s) override;
            void visit(s::VectorMaterialInputStruct& s) override;
            void visit(s::Vector2DStruct& s) override;
            void visit(s::Vector4Struct& s) override;
            void visit(s::VectorStruct& s) override;
        };
    };
} // namespace Satisfactory3DMap::UI
