#include "PropertyTableEditor.h"

#include <cinttypes>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <imgui.h>
#include <imgui_stdlib.h>

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

#include "Utils/ImGuiUtil.h"

namespace {
    bool InputFName(const char* label, SatisfactorySave::FName& name) {
        // TODO Number
        return ImGui::InputText(label, &name.Name);
    }

    class StructValueEditor : public SatisfactorySave::StructVisitor {
    private:
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit StructValueEditor(const std::function<void(const std::string&)>& callback) : callback_(callback) {}

        void visit(SatisfactorySave::BoxStruct& s) override {
            ImGui::Text("Min: %f %f %f", s.Data.Min.X, s.Data.Min.Y, s.Data.Min.Z);
            ImGui::Text("Max: %f %f %f", s.Data.Max.X, s.Data.Max.Y, s.Data.Max.Z);
            ImGui::Text("Valid: %i", s.Data.IsValid);
        }

        void visit(SatisfactorySave::ColorStruct& s) override {
            ImGui::Text("BGRA: %i %i %i %i", s.Data.B, s.Data.G, s.Data.R, s.Data.A);
        }

        void visit(SatisfactorySave::FluidBoxStruct& s) override {
            ImGui::Text("V: %f", s.Data.Value);
        }

        void visit(SatisfactorySave::GuidStruct& s) override {
            ImGui::Text("%s", s.Data.toString().c_str());
        }

        void visit(SatisfactorySave::IntPointStruct& s) override {
            ImGui::Text("X: %i  Y: %i", s.Data.X, s.Data.Y);
        }

        void visit(SatisfactorySave::IntVectorStruct& s) override {
            ImGui::Text("V: %i %i %i", s.Data.X, s.Data.Y, s.Data.Z);
        }

        void visit(SatisfactorySave::InventoryItemStruct& s) override {
            ImGui::Text("ItemClass:");
            ImGui::Text("Lvl:  %s", s.Data.ItemClass.LevelName.c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(s.Data.ItemClass.PathName, callback_);
            ImGui::Text("ItemState.ScriptStruct:");
            ImGui::Text("Lvl:  %s", s.Data.ItemState.ScriptStruct.LevelName.c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(s.Data.ItemState.ScriptStruct.PathName, callback_);
            ImGui::Text("ItemState.StructInstance:");
            Satisfactory3DMap::PropertyTableEditor r;
            r.renderGui(s.Data.ItemState.StructInstance, callback_);
            ImGui::Text("LegacyItemStateActor:");
            ImGui::Text("Lvl:  %s", s.Data.LegacyItemStateActor.LevelName.c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(s.Data.LegacyItemStateActor.PathName, callback_);
        }

        void visit(SatisfactorySave::LBBalancerIndexingStruct& s) override {
            ImGui::Text("NormalIdx: %i", s.Data.mNormalIndex);
            ImGui::Text("OverflowIdx: %i", s.Data.mOverflowIndex);
            ImGui::Text("FilterIdx: %i", s.Data.mFilterIndex);
        }

        void visit(SatisfactorySave::LinearColorStruct& s) override {
            ImGui::Text("RGBA: %f %f %f %f", s.Data.R, s.Data.G, s.Data.B, s.Data.A);
        }

        void visit(SatisfactorySave::PropertyStruct& s) override {
            Satisfactory3DMap::PropertyTableEditor r;
            r.renderGui(s.Data, callback_);
        }

        void visit(SatisfactorySave::QuatStruct& s) override {
            ImGui::Text("Q: %f %f %f %f", s.Data.X, s.Data.Y, s.Data.Z, s.Data.W);
        }

        void visit(SatisfactorySave::RailroadTrackPositionStruct& s) override {
            ImGui::Text("Lvl:  %s", s.Data.Track.LevelName.c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(s.Data.Track.PathName, callback_);
            ImGui::Text("Offs: %f", s.Data.Offset);
            ImGui::Text("Forw: %f", s.Data.Forward);
        }

        void visit(SatisfactorySave::RotatorStruct& s) override {
            ImGui::Text("Pitch: %f, Yaw: %f, Roll: %f", s.Data.Pitch, s.Data.Yaw, s.Data.Roll);
        }

        void visit(SatisfactorySave::ScalarMaterialInputStruct& s) override {
            ImGui::Text("OutputIndex: %i", s.Data.OutputIndex);
            ImGui::Text("InputName: %s", s.Data.InputName.toString().c_str());
            ImGui::Text("ExpressionName: %s", s.Data.ExpressionName.toString().c_str());
        }

        void visit(SatisfactorySave::SoftClassPathStruct& s) override {
            ImGui::Text("AssetPath.PackageName: %s", s.Data.AssetPath.PackageName.toString().c_str());
            ImGui::Text("AssetPath.AssetName: %s", s.Data.AssetPath.AssetName.toString().c_str());
            ImGui::Text("SubPathString: %s", s.Data.SubPathString.c_str());
        }

        void visit(SatisfactorySave::VectorMaterialInputStruct& s) override {
            ImGui::Text("OutputIndex: %i", s.Data.OutputIndex);
            ImGui::Text("InputName: %s", s.Data.InputName.toString().c_str());
            ImGui::Text("ExpressionName: %s", s.Data.ExpressionName.toString().c_str());
        }

        void visit(SatisfactorySave::Vector2DStruct& s) override {
            ImGui::Text("V: %f %f", s.Data.X, s.Data.Y);
        }

        void visit(SatisfactorySave::Vector4Struct& s) override {
            ImGui::Text("V: %f %f %f %f", s.Data.X, s.Data.Y, s.Data.Z, s.Data.W);
        }

        void visit(SatisfactorySave::VectorStruct& s) override {
            ImGui::Text("V: %f %f %f", s.Data.X, s.Data.Y, s.Data.Z);
        }
    };

    class ArraySetValueEditor : public SatisfactorySave::ArrayVisitor, public SatisfactorySave::SetVisitor {
    private:
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit ArraySetValueEditor(const std::function<void(const std::string&)>& callback) : callback_(callback) {}

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

        void visit(SatisfactorySave::Int8Array& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%" PRIi8, a.Values[i]);
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
                    ImGui::Text("Lvl:  %s", a.Values[i].LevelName.c_str());
                    ImGui::Text("Path:");
                    ImGui::SameLine();
                    Satisfactory3DMap::ImGuiUtil::PathLink(a.Values[i].PathName, callback_);
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
                    ImGui::Text("Lvl:  %s", a.Values[i].LevelName.c_str());
                    ImGui::Text("Path:");
                    ImGui::SameLine();
                    Satisfactory3DMap::ImGuiUtil::PathLink(a.Values[i].PathName, callback_);
                    ImGui::Text("Pak: %i", a.Values[i].pakValue());
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::SoftObjectArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("AssetPath.PackageName:  %s", a.Values[i].AssetPath.PackageName.toString().c_str());
                    ImGui::Text("AssetPath.AssetName:  %s", a.Values[i].AssetPath.AssetName.toString().c_str());
                    ImGui::Text("SubPathString:  %s", a.Values[i].SubPathString.c_str());
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
            StructValueEditor r(callback_);
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
            ImGui::Text("%s", s.getStructName().toString().c_str());
            StructValueEditor r(callback_);
            if (tableHead()) {
                for (std::size_t i = 0; i < s.Values.size(); i++) {
                    tableIndexCol(i);
                    s.Values[i]->accept(r);
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::UInt32Set& s) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < s.Values.size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%" PRIu32, s.Values[i]);
                }
                ImGui::EndTable();
            }
        }
    };

    class MapTypeValueEditor : public SatisfactorySave::MapTypeListVisitor {
    private:
        std::size_t idx_;
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit MapTypeValueEditor(std::size_t idx, const std::function<void(const std::string&)>& callback)
            : idx_(idx),
              callback_(callback) {}

        void visit(SatisfactorySave::ByteMapTypeList& m) override {
            ImGui::Text("%i", m.List[idx_]);
        }

        void visit(SatisfactorySave::EnumMapTypeList& m) override {
            ImGui::Text("%s", m.List[idx_].toString().c_str());
        }

        void visit(SatisfactorySave::FloatMapTypeList& m) override {
            ImGui::Text("%f", m.List[idx_]);
        }

        void visit(SatisfactorySave::Int64MapTypeList& m) override {
            ImGui::Text("%" PRIi64, m.List[idx_]);
        }

        void visit(SatisfactorySave::IntMapTypeList& m) override {
            ImGui::Text("%i", m.List[idx_]);
        }

        void visit(SatisfactorySave::NameMapTypeList& m) override {
            ImGui::Text("%s", m.List[idx_].toString().c_str());
        }

        void visit(SatisfactorySave::ObjectMapTypeList& m) override {
            ImGui::Text("Lvl:  %s", m.List[idx_].LevelName.c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(m.List[idx_].PathName, callback_);
        }

        void visit(SatisfactorySave::StructMapTypeList& m) override {
            StructValueEditor r(callback_);
            m.List[idx_]->accept(r);
        }
    };

    class PropertyValueEditor : public SatisfactorySave::PropertyVisitor {
    private:
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit PropertyValueEditor(const std::function<void(const std::string&)>& callback) : callback_(callback) {}

        void visit(SatisfactorySave::ArrayProperty& p) override {
            ImGui::TextDisabled("ArrayType: %s", p.arrayType().toString().c_str());
            ArraySetValueEditor r(callback_);
            p.Value->accept(r);
        }

        void visit(SatisfactorySave::BoolProperty& p) override {
            bool v = p.getValue();
            ImGui::Checkbox("##Value", &v);
            p.setValue(v);
        }

        void visit(SatisfactorySave::ByteProperty& p) override {
            ImGui::TextDisabled("EnumName: %s", p.enumName().toString().c_str());
            if (p.enumName() == "None") {
                ImGui::Text("%i", std::get<int8_t>(p.Value));
            } else {
                ImGui::Text("%s", std::get<SatisfactorySave::FName>(p.Value).toString().c_str());
            }
        }

        void visit(SatisfactorySave::DoubleProperty& p) override {
            ImGui::InputDouble("##Value", &p.Value);
        }

        void visit(SatisfactorySave::EnumProperty& p) override {
            ImGui::TextDisabled("EnumType: %s", p.enumName().toString().c_str());
            InputFName("##Value", p.Value);
        }

        void visit(SatisfactorySave::FloatProperty& p) override {
            ImGui::InputFloat("##Value", &p.Value);
        }

        void visit(SatisfactorySave::Int64Property& p) override {
            ImGui::InputScalar("##Value", ImGuiDataType_S64, &p.Value);
        }

        void visit(SatisfactorySave::Int8Property& p) override {
            ImGui::InputScalar("##Value", ImGuiDataType_S8, &p.Value);
        }

        void visit(SatisfactorySave::IntProperty& p) override {
            ImGui::InputInt("##Value", &p.Value);
        }

        void visit(SatisfactorySave::MapProperty& p) override {
            ImGui::TextDisabled("KeyType:   %s", p.keyType().toString().c_str());
            ImGui::TextDisabled("ValueType: %s", p.valueType().toString().c_str());

            auto& keys = *p.Keys;
            auto& values = *p.Values;
            if (keys.size() != values.size()) {
                throw std::runtime_error("Invalid MapProperty!");
            }
            auto size = keys.size();

            if (ImGui::BeginTable("tableMap", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableSetupColumn("Key");
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                for (std::size_t i = 0; i < size; i++) {
                    MapTypeValueEditor r(i, callback_);
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
                for (std::size_t i = 0; i < p.Value.InvocationList.size(); i++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%llu", i);
                    ImGui::TableNextColumn();
                    ImGui::Text("Object: %i", p.Value.InvocationList[i].Object.pakValue()); // TODO
                    ImGui::Text("FunctionName: %s", p.Value.InvocationList[i].FunctionName.toString().c_str());
                }
                ImGui::EndTable();
            }
        }

        void visit(SatisfactorySave::NameProperty& p) override {
            InputFName("##Value", p.Value);
        }

        void visit(SatisfactorySave::ObjectProperty& p) override {
            ImGui::InputText("Lvl", &p.Value.LevelName);
            ImGui::InputText("Path", &p.Value.PathName);
        }

        void visit(SatisfactorySave::SetProperty& p) override {
            ImGui::TextDisabled("SetType: %s", p.setType().toString().c_str());
            ArraySetValueEditor r(callback_);
            p.Value->accept(r);
        }

        void visit(SatisfactorySave::SoftObjectProperty& p) override {
            ImGui::Text("AssetPath.PackageName:  %s", p.Value.AssetPath.PackageName.toString().c_str());
            ImGui::Text("AssetPath.AssetName:  %s", p.Value.AssetPath.AssetName.toString().c_str());
            ImGui::Text("SubPathString:  %s", p.Value.SubPathString.c_str());
        }

        void visit(SatisfactorySave::StrProperty& p) override {
            ImGui::InputText("##Value", &p.Value);
        }

        void visit(SatisfactorySave::StructProperty& p) override {
            ImGui::TextDisabled("StructName:");
            ImGui::SameLine();
            ImGui::Text("%s", p.structName().toString().c_str());
            ImGui::TextDisabled("%s", p.structGuid().toString().c_str());
            StructValueEditor s(callback_);
            p.Value->accept(s);
        }

        void visit(SatisfactorySave::TextProperty& p) override {
            ImGui::Text("%s", p.Value.string().c_str());
        }

        void visit(SatisfactorySave::UInt32Property& p) override {
            ImGui::InputScalar("##Value", ImGuiDataType_U32, &p.Value);
        }

        void visit(SatisfactorySave::UInt64Property& p) override {
            ImGui::InputScalar("##Value", ImGuiDataType_U64, &p.Value);
        }

        void visit(SatisfactorySave::UnknownProperty& p) override {
            ImGui::Text("[UnknownProperty] %s, size: %zu", p.type().toString().c_str(), p.Value.size());
            if (ImGui::SmallButton(("Copy Hex##" + p.name().toString()).c_str())) {
                std::stringstream stream;
                for (const auto& c : p.Value) {
                    stream << std::setfill('0') << std::setw(2) << std::hex
                           << static_cast<int>(static_cast<unsigned char>(c)) << " ";
                }
                std::string hex_data = stream.str();
                ImGui::SetClipboardText(hex_data.c_str());
            }
        }
    };
} // namespace

void Satisfactory3DMap::PropertyTableEditor::renderGui(const SatisfactorySave::PropertyList& properties,
    const std::function<void(const std::string&)>& callback) {
    if (ImGui::BeginTable("tableProperties", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Property");
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();
        for (const auto& p : properties) {
            ImGui::PushID(p.get());
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::PushItemWidth(150.0f);
            InputFName("##Name", p->name());
            ImGui::PopItemWidth();
            ImGui::TextDisabled("%s", p->type().toString().c_str());
            ImGui::TableNextColumn();
            PropertyValueEditor r(callback);
            p->accept(r);
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}
