#include "PropertyTableGuiRenderer.h"

#include <cinttypes>
#include <stdexcept>

#include <imgui.h>

#include "GameTypes/Arrays/ArrayVisitor.h"
#include "GameTypes/MapTypes/MapTypeListVisitor.h"
#include "GameTypes/Properties/PropertyVisitor.h"
#include "GameTypes/Structs/StructVisitor.h"
#include "Utils/ImGuiUtil.h"

namespace {
    class StructValueGuiRenderer : public Satisfactory3DMap::StructVisitor {
    private:
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit StructValueGuiRenderer(const std::function<void(const std::string&)>& callback)
            : callback_(callback) {}

        void visit(Satisfactory3DMap::BoxStruct& s) override {
            ImGui::Text("Min: %f %f %f", s.min().x, s.min().y, s.min().z);
            ImGui::Text("Max: %f %f %f", s.max().x, s.max().y, s.max().z);
            ImGui::Text("Valid: %i", s.isValid());
        }

        void visit(Satisfactory3DMap::ColorStruct& s) override {
            ImGui::Text("BGRA: %i %i %i %i", s.b(), s.g(), s.r(), s.a());
        }

        void visit(Satisfactory3DMap::FluidBoxStruct& s) override {
            ImGui::Text("V: %f", s.value());
        }

        void visit([[maybe_unused]] Satisfactory3DMap::GuidStruct& g) override {
            // TODO
        }

        void visit([[maybe_unused]] Satisfactory3DMap::IntPointStruct& p) override {
            // TODO
        }

        void visit(Satisfactory3DMap::InventoryItemStruct& s) override {
            ImGui::Text("U: %i", s.unk1());
            ImGui::Text("C: %s", s.className().c_str());
            ImGui::Text("Lvl:  %s", s.ref().levelName().c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(s.ref().pathName(), callback_);
        }

        void visit(Satisfactory3DMap::LinearColorStruct& s) override {
            ImGui::Text("RGBA: %f %f %f %f", s.r(), s.g(), s.b(), s.a());
        }

        void visit(Satisfactory3DMap::PropertyStruct& s) override {
            Satisfactory3DMap::PropertyTableGuiRenderer r;
            r.renderGui(s.properties(), callback_);
        }

        void visit(Satisfactory3DMap::QuatStruct& s) override {
            ImGui::Text("Q: %f %f %f %f", s.x(), s.y(), s.z(), s.w());
        }

        void visit(Satisfactory3DMap::RailroadTrackPositionStruct& s) override {
            ImGui::Text("Lvl:  %s", s.ref().levelName().c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(s.ref().pathName(), callback_);
            ImGui::Text("Offs: %f", s.offset());
            ImGui::Text("Forw: %f", s.forward());
        }

        void visit(Satisfactory3DMap::RotatorStruct& s) override {
            ImGui::Text("Pitch: %f, Yaw: %f, Roll: %f", s.pitch(), s.yaw(), s.roll());
        }

        void visit(Satisfactory3DMap::VectorStruct& s) override {
            ImGui::Text("V: %f %f %f", s.value().x, s.value().y, s.value().z);
        }
    };

    class ArrayValueGuiRenderer : public Satisfactory3DMap::ArrayVisitor {
    private:
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit ArrayValueGuiRenderer(const std::function<void(const std::string&)>& callback) : callback_(callback) {}

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
                    ImGui::Text("Lvl:  %s", a.array()[i].levelName().c_str());
                    ImGui::Text("Path:");
                    ImGui::SameLine();
                    Satisfactory3DMap::ImGuiUtil::PathLink(a.array()[i].pathName(), callback_);
                    ImGui::Text("Pak: %i", a.array()[i].pakValue());
                }
                ImGui::EndTable();
            }
        }

        void visit(Satisfactory3DMap::StructArray& a) override {
            ImGui::TextDisabled("StructName:");
            ImGui::SameLine();
            ImGui::Text("%s", a.structName().toString().c_str());
            ImGui::TextDisabled("%s", a.guid().toString().c_str());
            StructValueGuiRenderer r(callback_);
            if (tableHead()) {
                for (std::size_t i = 0; i < a.array().size(); i++) {
                    tableIndexCol(i);
                    a.array()[i]->accept(r);
                }
                ImGui::EndTable();
            }
        }
    };

    class MapTypeValueGuiRenderer : public Satisfactory3DMap::MapTypeListVisitor {
    private:
        std::size_t idx_;
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit MapTypeValueGuiRenderer(std::size_t idx, const std::function<void(const std::string&)>& callback)
            : idx_(idx),
              callback_(callback) {}

        void visit(Satisfactory3DMap::ByteMapTypeList& m) override {
            ImGui::Text("%i", m.list()[idx_]);
        }

        void visit(Satisfactory3DMap::EnumMapTypeList& m) override {
            ImGui::Text("%s", m.list()[idx_].c_str());
        }

        void visit(Satisfactory3DMap::IntMapTypeList& m) override {
            ImGui::Text("%i", m.list()[idx_]);
        }

        void visit(Satisfactory3DMap::ObjectMapTypeList& m) override {
            ImGui::Text("Lvl:  %s", m.list()[idx_].levelName().c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(m.list()[idx_].pathName(), callback_);
        }

        void visit(Satisfactory3DMap::StructMapTypeList& m) override {
            StructValueGuiRenderer r(callback_);
            m.list()[idx_]->accept(r);
        }
    };

    class PropertyValueGuiRenderer : public Satisfactory3DMap::PropertyVisitor {
    private:
        const std::function<void(const std::string&)>& callback_;

    public:
        explicit PropertyValueGuiRenderer(const std::function<void(const std::string&)>& callback)
            : callback_(callback) {}

        void visit(Satisfactory3DMap::ArrayProperty& p) override {
            ImGui::TextDisabled("ArrayType: %s", p.arrayType().toString().c_str());
            ArrayValueGuiRenderer r(callback_);
            p.array()->accept(r);
        }

        void visit(Satisfactory3DMap::BoolProperty& p) override {
            ImGui::Text("%i", p.value());
        }

        void visit(Satisfactory3DMap::ByteProperty& p) override {
            ImGui::TextDisabled("ByteType: %s", p.byteType().toString().c_str());
            if (p.byteType() == "None") {
                ImGui::Text("%i", p.valueByte());
            } else {
                ImGui::Text("%s", p.valueName().toString().c_str());
            }
        }

        void visit(Satisfactory3DMap::EnumProperty& p) override {
            ImGui::TextDisabled("EnumType: %s", p.enumType().toString().c_str());
            ImGui::Text("%s", p.value().toString().c_str());
        }

        void visit(Satisfactory3DMap::FloatProperty& p) override {
            ImGui::Text("%f", p.value());
        }

        void visit(Satisfactory3DMap::Int64Property& p) override {
            ImGui::Text("%" PRIi64, p.value());
        }

        void visit(Satisfactory3DMap::Int8Property& p) override {
            ImGui::Text("%i", p.value());
        }

        void visit(Satisfactory3DMap::IntProperty& p) override {
            ImGui::Text("%i", p.value());
        }

        void visit(Satisfactory3DMap::MapProperty& p) override {
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

        void visit(Satisfactory3DMap::NameProperty& p) override {
            ImGui::Text("%s", p.value().toString().c_str());
        }

        void visit(Satisfactory3DMap::ObjectProperty& p) override {
            ImGui::Text("Lvl:  %s", p.value().levelName().c_str());
            ImGui::Text("Path:");
            ImGui::SameLine();
            Satisfactory3DMap::ImGuiUtil::PathLink(p.value().pathName(), callback_);
            ImGui::Text("Pak: %i", p.value().pakValue());
        }

        void visit(Satisfactory3DMap::StrProperty& p) override {
            ImGui::Text("%s", p.value().c_str());
        }

        void visit(Satisfactory3DMap::StructProperty& p) override {
            ImGui::TextDisabled("StructName:");
            ImGui::SameLine();
            ImGui::Text("%s", p.structName().c_str());
            ImGui::TextDisabled("%s", p.guid().toString().c_str());
            StructValueGuiRenderer s(callback_);
            p.value()->accept(s);
        }

        void visit([[maybe_unused]] Satisfactory3DMap::TextProperty& p) override {
            ImGui::Text("TODO!");
        }
    };
} // namespace

void Satisfactory3DMap::PropertyTableGuiRenderer::renderGui(const Properties& properties,
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
