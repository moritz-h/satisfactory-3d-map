#include "PropertyTableGuiRenderer.h"

#include <cinttypes>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <imgui.h>

#include "SatisfactorySave/GameTypes/Arrays/Base/ArrayAll.h"
#include "SatisfactorySave/GameTypes/Arrays/Base/ArrayVisitor.h"
#include "SatisfactorySave/GameTypes/MapTypes/MapTypeListVisitor.h"
#include "SatisfactorySave/GameTypes/Properties/PropertyVisitor.h"
#include "SatisfactorySave/GameTypes/Sets/SetVisitor.h"
#include "SatisfactorySave/GameTypes/Structs/Base/StructAll.h"
#include "SatisfactorySave/GameTypes/Structs/Base/StructVisitor.h"

#include "Utils/ImGuiUtil.h"

namespace {
    class StructValueGuiRenderer : public SatisfactorySave::StructVisitor {
    private:
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit StructValueGuiRenderer(const std::function<void(const std::string&)>& callback)
            : callback_(callback) {}

        void visit(SatisfactorySave::BoxStruct& s) override {
            ImGui::Text("Min: %f %f %f", s.min().x, s.min().y, s.min().z);
            ImGui::Text("Max: %f %f %f", s.max().x, s.max().y, s.max().z);
            ImGui::Text("Valid: %i", s.isValid());
        }

        void visit(SatisfactorySave::ColorStruct& s) override {
            ImGui::Text("BGRA: %i %i %i %i", s.b(), s.g(), s.r(), s.a());
        }

        void visit(SatisfactorySave::FluidBoxStruct& s) override {
            ImGui::Text("V: %f", s.value());
        }

        void visit(SatisfactorySave::GuidStruct& s) override {
            ImGui::Text("%s", s.guid().toString().c_str());
        }

        void visit(SatisfactorySave::IntPointStruct& s) override {
            ImGui::Text("X: %i  Y: %i", s.x(), s.y());
        }

        void visit(SatisfactorySave::IntVectorStruct& s) override {
            ImGui::Text("V: %i %i %i", s.value().x, s.value().y, s.value().z);
        }

        void visit(SatisfactorySave::InventoryItemStruct& s) override {
            ImGui::Text("U: %i", s.unk1());
            ImGui::Text("C: %s", s.className().c_str());
            ImGui::Text("Lvl:  %s", s.ref().levelName().c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(s.ref().pathName(), callback_);
        }

        void visit(SatisfactorySave::LBBalancerIndexingStruct& s) override {
            ImGui::Text("NormalIdx: %i", s.normalIndex());
            ImGui::Text("OverflowIdx: %i", s.overflowIndex());
            ImGui::Text("FilterIdx: %i", s.filterIndex());
        }

        void visit(SatisfactorySave::LinearColorStruct& s) override {
            ImGui::Text("RGBA: %f %f %f %f", s.r(), s.g(), s.b(), s.a());
        }

        void visit(SatisfactorySave::PropertyStruct& s) override {
            Satisfactory3DMap::PropertyTableGuiRenderer r;
            r.renderGui(s.properties(), callback_);
        }

        void visit(SatisfactorySave::QuatStruct& s) override {
            ImGui::Text("Q: %f %f %f %f", s.x(), s.y(), s.z(), s.w());
        }

        void visit(SatisfactorySave::RailroadTrackPositionStruct& s) override {
            ImGui::Text("Lvl:  %s", s.ref().levelName().c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(s.ref().pathName(), callback_);
            ImGui::Text("Offs: %f", s.offset());
            ImGui::Text("Forw: %f", s.forward());
        }

        void visit(SatisfactorySave::RotatorStruct& s) override {
            ImGui::Text("Pitch: %f, Yaw: %f, Roll: %f", s.pitch(), s.yaw(), s.roll());
        }

        void visit(SatisfactorySave::ScalarMaterialInputStruct& s) override {
            ImGui::Text("OutputIndex: %i", s.OutputIndex());
            ImGui::Text("InputName: %s", s.InputName().toString().c_str());
            ImGui::Text("ExpressionName: %s", s.ExpressionName().toString().c_str());
        }

        void visit(SatisfactorySave::SoftClassPathStruct& s) override {
            ImGui::Text("AssetPathName: %s", s.AssetPathName().toString().c_str());
            ImGui::Text("SubPathString: %s", s.SubPathString().c_str());
        }

        void visit(SatisfactorySave::VectorMaterialInputStruct& s) override {
            ImGui::Text("OutputIndex: %i", s.OutputIndex());
            ImGui::Text("InputName: %s", s.InputName().toString().c_str());
            ImGui::Text("ExpressionName: %s", s.ExpressionName().toString().c_str());
        }

        void visit(SatisfactorySave::Vector2DStruct& s) override {
            ImGui::Text("V: %f %f", s.value().x, s.value().y);
        }

        void visit(SatisfactorySave::VectorStruct& s) override {
            ImGui::Text("V: %f %f %f", s.value().x, s.value().y, s.value().z);
        }
    };

    class ArraySetValueGuiRenderer : public SatisfactorySave::ArrayVisitor, public SatisfactorySave::SetVisitor {
    private:
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit ArraySetValueGuiRenderer(const std::function<void(const std::string&)>& callback)
            : callback_(callback) {}

        static bool tableHead() {
            if (ImGui::BeginTable("tableArray", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Idx");
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                return true;
            }
            return false;
        }

        static void tableIndexCol(std::size_t i) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%lu", i);
            ImGui::TableNextColumn();
        }

        void visit(SatisfactorySave::BoolArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%i", a.Values[i]);
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::ByteArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%i", a.Values[i]);
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::EnumArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%s", a.Values[i].toString().c_str());
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::FloatArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%f", a.Values[i]);
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::Int64Array& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%" PRIi64, a.Values[i]);
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::IntArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%i", a.Values[i]);
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::InterfaceArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("Lvl:  %s", a.Values[i].levelName().c_str());
                    ImGui::Text("Path:");
                    ImGui::SameLine();
                    Satisfactory3DMap::ImGuiUtil::PathLink(a.Values[i].pathName(), callback_);
                    ImGui::Text("Pak: %i", a.Values[i].pakValue());
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::NameArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%s", a.Values[i].toString().c_str());
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::ObjectArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("Lvl:  %s", a.Values[i].levelName().c_str());
                    ImGui::Text("Path:");
                    ImGui::SameLine();
                    Satisfactory3DMap::ImGuiUtil::PathLink(a.Values[i].pathName(), callback_);
                    ImGui::Text("Pak: %i", a.Values[i].pakValue());
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::SoftObjectArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("AssetPathName:  %s", a.Values[i].AssetPathName().toString().c_str());
                    ImGui::Text("SubPathString:  %s", a.Values[i].SubPathString().c_str());
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::StrArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%s", a.Values[i].c_str());
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::StructArray& a) override {
            ImGui::TextDisabled("StructName:");
            ImGui::SameLine();
            ImGui::Text("%s", a.structName().toString().c_str());
            ImGui::TextDisabled("%s", a.structGuid().toString().c_str());
            StructValueGuiRenderer r(callback_);
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    a.Values[i]->accept(r);
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::StructSet& s) override {
            ImGui::TextDisabled("StructName:");
            ImGui::SameLine();
            ImGui::Text("%s", s.structName().toString().c_str());
            StructValueGuiRenderer r(callback_);
            if (tableHead()) {
                for (std::size_t i = 0; i < s.set().size(); i++) {
                    tableIndexCol(i);
                    s.set()[i]->accept(r);
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::UInt32Set& s) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < s.set().size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%" PRIu32, s.set()[i]);
                }
                ImGui::EndTable();
            }
        }
    };

    class MapTypeValueGuiRenderer : public SatisfactorySave::MapTypeListVisitor {
    private:
        std::size_t idx_;
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit MapTypeValueGuiRenderer(std::size_t idx, const std::function<void(const std::string&)>& callback)
            : idx_(idx),
              callback_(callback) {}

        void visit(SatisfactorySave::ByteMapTypeList& m) override {
            ImGui::Text("%i", m.list()[idx_]);
        }

        void visit(SatisfactorySave::EnumMapTypeList& m) override {
            ImGui::Text("%s", m.list()[idx_].c_str());
        }

        void visit(SatisfactorySave::FloatMapTypeList& m) override {
            ImGui::Text("%f", m.list()[idx_]);
        }

        void visit(SatisfactorySave::IntMapTypeList& m) override {
            ImGui::Text("%i", m.list()[idx_]);
        }

        void visit(SatisfactorySave::NameMapTypeList& m) override {
            ImGui::Text("%s", m.list()[idx_].toString().c_str());
        }

        void visit(SatisfactorySave::ObjectMapTypeList& m) override {
            ImGui::Text("Lvl:  %s", m.list()[idx_].levelName().c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(m.list()[idx_].pathName(), callback_);
        }

        void visit(SatisfactorySave::StructMapTypeList& m) override {
            StructValueGuiRenderer r(callback_);
            m.list()[idx_]->accept(r);
        }
    };

    class PropertyValueGuiRenderer : public SatisfactorySave::PropertyVisitor {
    private:
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit PropertyValueGuiRenderer(const std::function<void(const std::string&)>& callback)
            : callback_(callback) {}

        void visit(SatisfactorySave::ArrayProperty& p) override {
            ImGui::TextDisabled("ArrayType: %s", p.arrayType().toString().c_str());
            ArraySetValueGuiRenderer r(callback_);
            p.array()->accept(r);
        }

        void visit(SatisfactorySave::BoolProperty& p) override {
            ImGui::Text("%i", p.value());
        }

        void visit(SatisfactorySave::ByteProperty& p) override {
            ImGui::TextDisabled("ByteType: %s", p.byteType().toString().c_str());
            if (p.byteType() == "None") {
                ImGui::Text("%i", p.valueByte());
            } else {
                ImGui::Text("%s", p.valueName().toString().c_str());
            }
        }

        void visit(SatisfactorySave::DoubleProperty& p) override {
            ImGui::Text("%f", p.value());
        }

        void visit(SatisfactorySave::EnumProperty& p) override {
            ImGui::TextDisabled("EnumType: %s", p.enumType().toString().c_str());
            ImGui::Text("%s", p.value().toString().c_str());
        }

        void visit(SatisfactorySave::FloatProperty& p) override {
            ImGui::Text("%f", p.value());
        }

        void visit(SatisfactorySave::Int64Property& p) override {
            ImGui::Text("%" PRIi64, p.value());
        }

        void visit(SatisfactorySave::Int8Property& p) override {
            ImGui::Text("%i", p.value());
        }

        void visit(SatisfactorySave::IntProperty& p) override {
            ImGui::Text("%i", p.value());
        }

        void visit(SatisfactorySave::MapProperty& p) override {
            ImGui::TextDisabled("KeyType:   %s", p.keyType().toString().c_str());
            ImGui::TextDisabled("ValueType: %s", p.valueType().toString().c_str());

            auto& keys = *p.keys();
            auto& values = *p.values();
            if (keys.listSize() != values.listSize()) {
                throw std::runtime_error("Invalid MapProperty!");
            }
            auto size = keys.listSize();

            if (ImGui::BeginTable("tableMap", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Key");
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                for (std::size_t i = 0; i < size; i++) {
                    MapTypeValueGuiRenderer r(i, callback_);
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    keys.accept(r);
                    ImGui::TableNextColumn();
                    values.accept(r);
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::MulticastSparseDelegateProperty& p) override {
            if (ImGui::BeginTable("tableMulticastSparseDelegate", 2,
                    ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Idx");
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                for (std::size_t i = 0; i < p.InvocationList().size(); i++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%llu", i);
                    ImGui::TableNextColumn();
                    ImGui::Text("Object: %i", p.InvocationList()[i].Object.pakValue()); // TODO
                    ImGui::Text("FunctionName: %s", p.InvocationList()[i].FunctionName.toString().c_str());
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::NameProperty& p) override {
            ImGui::Text("%s", p.value().toString().c_str());
        }

        void visit(SatisfactorySave::ObjectProperty& p) override {
            ImGui::Text("Lvl:  %s", p.value().levelName().c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(p.value().pathName(), callback_);
            ImGui::Text("Pak: %i", p.value().pakValue());
        }

        void visit(SatisfactorySave::SetProperty& p) override {
            ImGui::TextDisabled("SetType: %s", p.setType().toString().c_str());
            ArraySetValueGuiRenderer r(callback_);
            p.set()->accept(r);
        }

        void visit(SatisfactorySave::SoftObjectProperty& p) override {
            ImGui::Text("AssetPathName:  %s", p.value().AssetPathName().toString().c_str());
            ImGui::Text("SubPathString:  %s", p.value().SubPathString().c_str());
        }

        void visit(SatisfactorySave::StrProperty& p) override {
            ImGui::Text("%s", p.value().c_str());
        }

        void visit(SatisfactorySave::StructProperty& p) override {
            ImGui::TextDisabled("StructName:");
            ImGui::SameLine();
            ImGui::Text("%s", p.structName().c_str());
            ImGui::TextDisabled("%s", p.guid().toString().c_str());
            StructValueGuiRenderer s(callback_);
            p.value()->accept(s);
        }

        void visit(SatisfactorySave::TextProperty& p) override {
            ImGui::Text("%s", p.textString().c_str());
        }

        void visit(SatisfactorySave::UInt32Property& p) override {
            ImGui::Text("%" PRIu32, p.value());
        }

        void visit(SatisfactorySave::UInt64Property& p) override {
            ImGui::Text("%" PRIu64, p.value());
        }

        void visit(SatisfactorySave::UnknownProperty& p) override {
            ImGui::Text("[UnknownProperty] %s, size: %zu", p.type().toString().c_str(), p.value().size());
            if (ImGui::SmallButton(("Copy Hex##" + p.name().toString()).c_str())) {
                std::stringstream stream;
                for (const auto& c : p.value()) {
                    stream << std::setfill('0') << std::setw(2) << std::hex
                           << static_cast<int>(static_cast<unsigned char>(c)) << " ";
                }
                std::string hex_data = stream.str();
                ImGui::SetClipboardText(hex_data.c_str());
            }
        }
    };
} // namespace

void Satisfactory3DMap::PropertyTableGuiRenderer::renderGui(const SatisfactorySave::Properties& properties,
    const std::function<void(const std::string&)>& callback) {
    if (ImGui::BeginTable("tableProperties", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Property");
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        for (const auto& p : properties) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%s", p->name().toString().c_str());
            ImGui::TextDisabled("%s", p->type().toString().c_str());
            ImGui::TableNextColumn();
            PropertyValueGuiRenderer r(callback);
            p->accept(r);
        }
        ImGui::EndTable();
    }
}
