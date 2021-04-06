#include "PropertyTableGuiRenderer.h"

#include <imgui.h>

#include "SaveGame/Types/Properties/PropertyVisitor.h"
#include "SaveGame/Types/Structs/StructVisitor.h"
#include "Utils/ResourceUtils.h"

namespace {
    class StructValueGuiRenderer : public Satisfactory3DMap::StructVisitor {
        void visit(Satisfactory3DMap::BoxStruct& p) override {
            // TODO
            ImGui::Text("TODO!");
        }
        void visit(Satisfactory3DMap::ColorStruct& p) override {
            // TODO
            ImGui::Text("TODO!");
        }
        void visit(Satisfactory3DMap::FluidBoxStruct& p) override {
            // TODO
            ImGui::Text("TODO!");
        }
        void visit(Satisfactory3DMap::InventoryItemStruct& p) override {
            // TODO
            ImGui::Text("TODO!");
        }
        void visit(Satisfactory3DMap::LinearColorStruct& p) override {
            // TODO
            ImGui::Text("TODO!");
        }
        void visit(Satisfactory3DMap::PropertyStruct& p) override {
            // TODO
            ImGui::Text("TODO!");
        }
        void visit(Satisfactory3DMap::QuatStruct& p) override {
            // TODO
            ImGui::Text("TODO!");
        }
        void visit(Satisfactory3DMap::RailroadTrackPositionStruct& p) override {
            // TODO
            ImGui::Text("TODO!");
        }
        void visit(Satisfactory3DMap::VectorStruct& p) override {
            // TODO
            ImGui::Text("TODO!");
        }
    };

    class PropertyValueGuiRenderer : public Satisfactory3DMap::PropertyVisitor {
        void visit(Satisfactory3DMap::ArrayProperty& p) override {
            ImGui::Text("T: %s", p.arrayType().c_str());
            ImGui::Text("TODO!");
        }

        void visit(Satisfactory3DMap::BoolProperty& p) override {
            ImGui::Text("%i", p.value());
        }

        void visit(Satisfactory3DMap::ByteProperty& p) override {
            ImGui::Text("T: %s", p.byteType().c_str());
            if (p.byteType() == "None") {
                ImGui::Text("V: %i", p.value()[0]);
            } else {
                ImGui::Text("V: %s", p.value().c_str());
            }
        }

        void visit(Satisfactory3DMap::EnumProperty& p) override {
            ImGui::Text("T: %s", p.enumType().c_str());
            ImGui::Text("V: %s", p.value().c_str());
        }

        void visit(Satisfactory3DMap::FloatProperty& p) override {
            ImGui::Text("%f", p.value());
        }

        void visit(Satisfactory3DMap::Int64Property& p) override {
            ImGui::Text("%lli", p.value());
        }

        void visit(Satisfactory3DMap::Int8Property& p) override {
            ImGui::Text("%i", p.value());
        }

        void visit(Satisfactory3DMap::IntProperty& p) override {
            ImGui::Text("%i", p.value());
        }

        void visit(Satisfactory3DMap::MapProperty& p) override {
            ImGui::Text("KT: %s", p.keyType().c_str());
            ImGui::Text("VT: %s", p.valueType().c_str());
            ImGui::Text("TODO!");
        }

        void visit(Satisfactory3DMap::NameProperty& p) override {
            ImGui::Text("%s", p.value().c_str());
        }

        void visit(Satisfactory3DMap::ObjectProperty& p) override {
            ImGui::Text("L: %s", p.value().levelName().c_str());
            ImGui::Text("P: %s", p.value().pathName().c_str());
        }

        void visit(Satisfactory3DMap::StrProperty& p) override {
            ImGui::Text("%s", p.value().c_str());
        }

        void visit(Satisfactory3DMap::StructProperty& p) override {
            ImGui::Text("N: %s", p.structName().c_str());
            ImGui::Text("G: %s", p.guid().c_str());
            StructValueGuiRenderer s;
            p.value()->accept(s);
        }

        void visit(Satisfactory3DMap::TextProperty& p) override {
            ImGui::Text("TODO!");
            if (ImGui::Button(("Show Hex##" + p.name()).c_str())) {
                // TODO
                // hexEditData_ = p.buf();
                // showHexEdit_ = true;
            }
        }
    };
} // namespace

void Satisfactory3DMap::PropertyTableGuiRenderer::renderGui(const std::vector<std::unique_ptr<Property>>& properties) {
    if (ImGui::BeginTable("tableProperties", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Type");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();
        for (const auto& p : properties) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%s", p->name().c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%s", p->type().c_str());
            ImGui::TableNextColumn();
            PropertyValueGuiRenderer r;
            p->accept(r);
        }
        ImGui::EndTable();
    }
}
