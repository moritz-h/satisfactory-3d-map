#include "PropertyTableGuiRenderer.h"

#include <imgui.h>

#include "SaveGame/Types/Arrays/ArrayVisitor.h"
#include "SaveGame/Types/Properties/PropertyVisitor.h"
#include "SaveGame/Types/Structs/StructVisitor.h"
#include "Utils/ResourceUtils.h"

namespace {
    class StructValueGuiRenderer : public Satisfactory3DMap::StructVisitor {
        void visit(Satisfactory3DMap::BoxStruct& s) override {
            ImGui::Text("Min: %f %f %f", s.min().x, s.min().y, s.min().z);
            ImGui::Text("Max: %f %f %f", s.max().x, s.max().y, s.max().z);
            ImGui::Text("V: %i", s.isValid());
        }

        void visit(Satisfactory3DMap::ColorStruct& s) override {
            ImGui::Text("BGRA: %i %i %i %i", s.b(), s.g(), s.r(), s.a());
        }

        void visit(Satisfactory3DMap::FluidBoxStruct& s) override {
            ImGui::Text("V: %f", s.value());
        }

        void visit(Satisfactory3DMap::InventoryItemStruct& s) override {
            ImGui::Text("U: %i", s.unk1());
            ImGui::Text("C: %s", s.className().c_str());
            ImGui::Text("L: %s", s.ref().levelName().c_str());
            ImGui::Text("P: %s", s.ref().pathName().c_str());
        }

        void visit(Satisfactory3DMap::LinearColorStruct& s) override {
            ImGui::Text("RGBA: %f %f %f %f", s.r(), s.g(), s.b(), s.a());
        }

        void visit(Satisfactory3DMap::PropertyStruct& s) override {
            Satisfactory3DMap::PropertyTableGuiRenderer r;
            r.renderGui(s.properties());
        }

        void visit(Satisfactory3DMap::QuatStruct& s) override {
            ImGui::Text("Q: %f %f %f %f", s.x(), s.y(), s.z(), s.w());
        }

        void visit(Satisfactory3DMap::RailroadTrackPositionStruct& s) override {
            ImGui::Text("L: %s", s.ref().levelName().c_str());
            ImGui::Text("P: %s", s.ref().pathName().c_str());
            ImGui::Text("O: %f", s.offset());
            ImGui::Text("F: %f", s.forward());
        }

        void visit(Satisfactory3DMap::VectorStruct& s) override {
            ImGui::Text("V: %f %f %f", s.value().x, s.value().y, s.value().z);
        }
    };

    class ArrayValueGuiRenderer : public Satisfactory3DMap::ArrayVisitor {
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
            ImGui::Text("%lli", i);
            ImGui::TableNextColumn();
        }

        void visit(Satisfactory3DMap::ByteArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.array().size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%i", a.array()[i]);
                }
                ImGui::EndTable();
            }
        }

        void visit(Satisfactory3DMap::EnumArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.array().size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%s", a.array()[i].c_str());
                }
                ImGui::EndTable();
            }
        }

        void visit(Satisfactory3DMap::IntArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.array().size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("%i", a.array()[i]);
                }
                ImGui::EndTable();
            }
        }

        void visit(Satisfactory3DMap::ObjectArray& a) override {
            if (tableHead()) {
                for (std::size_t i = 0; i < a.array().size(); i++) {
                    tableIndexCol(i);
                    ImGui::Text("L: %s", a.array()[i].levelName().c_str());
                    ImGui::Text("P: %s", a.array()[i].pathName().c_str());
                }
                ImGui::EndTable();
            }
        }

        void visit(Satisfactory3DMap::StructArray& a) override {
            StructValueGuiRenderer r;
            if (tableHead()) {
                for (std::size_t i = 0; i < a.array().size(); i++) {
                    tableIndexCol(i);
                    a.array()[i]->accept(r);
                }
                ImGui::EndTable();
            }
        }
    };

    class PropertyValueGuiRenderer : public Satisfactory3DMap::PropertyVisitor {
        void visit(Satisfactory3DMap::ArrayProperty& p) override {
            ImGui::Text("T: %s", p.arrayType().c_str());
            ArrayValueGuiRenderer r;
            p.array()->accept(r);
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
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
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
