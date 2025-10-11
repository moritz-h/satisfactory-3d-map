#include "PropertyEditor.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#include <imgui.h>

#include "ObjectWidgets.h"

void Satisfactory3DMap::UI::PropertyEditor::visit(s::ArrayProperty& p) {
    ImGui::BeginDisabled();
    EditorShowText("ArrayType", p.ArrayType().toString().c_str());
    ImGui::EndDisabled();
    ArrayEditor editor(*this);
    p.Value->accept(editor);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::BoolProperty& p) {
    bool v = p.getValue();
    if (EditorBool("Value", v)) {
        changed_ = true;
        p.setValue(v);
    }
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::ByteProperty& p) {
    ImGui::BeginDisabled();
    EditorShowText("EnumName", p.EnumName().toString().c_str());
    ImGui::EndDisabled();
    if (p.EnumName() == "None") {
        static constexpr int8_t step = 1;
        static constexpr int8_t step_fast = 10;
        changed_ |= EditorScalar("Value", ImGuiDataType_S8, &std::get<int8_t>(p.Value), &step, &step_fast);
    } else {
        changed_ |= EditorName("Value", std::get<s::FName>(p.Value));
    }
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::DoubleProperty& p) {
    static constexpr double step = 1.0;
    static constexpr double step_fast = 100.0;
    changed_ |= EditorScalar("Value", ImGuiDataType_Double, &p.Value, &step, &step_fast);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::EnumProperty& p) {
    ImGui::BeginDisabled();
    EditorShowText("EnumName", p.EnumName().toString().c_str());
    ImGui::EndDisabled();
    changed_ |= EditorName("Value", p.Value);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::FloatProperty& p) {
    static constexpr float step = 1.0f;
    static constexpr float step_fast = 100.0f;
    changed_ |= EditorScalar("Value", ImGuiDataType_Float, &p.Value, &step, &step_fast);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::Int64Property& p) {
    static constexpr int64_t step = 1;
    static constexpr int64_t step_fast = 100;
    changed_ |= EditorScalar("Value", ImGuiDataType_S64, &p.Value, &step, &step_fast);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::Int8Property& p) {
    static constexpr int8_t step = 1;
    static constexpr int8_t step_fast = 10;
    changed_ |= EditorScalar("Value", ImGuiDataType_S8, &p.Value, &step, &step_fast);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::IntProperty& p) {
    static constexpr int32_t step = 1;
    static constexpr int32_t step_fast = 100;
    changed_ |= EditorScalar("Value", ImGuiDataType_S32, &p.Value, &step, &step_fast);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::MapProperty& p) {
    ImGui::BeginDisabled();
    EditorShowText("KeyType", p.KeyType().toString().c_str());
    EditorShowText("ValueType", p.ValueType().toString().c_str());
    ImGui::EndDisabled();
    if (p.Keys->size() != p.Values->size()) {
        throw std::runtime_error("Invalid MapProperty!");
    }
    if (p.Keys->size() == 0) {
        EditorShowText("Value", "Map (Empty)");
    } else {
        const bool open = EditorTreeNode("Value", ImGuiTreeNodeFlags_DefaultOpen);
        ImGui::TableNextColumn();
        ImGui::Text("Map (Size: %zu)", p.Keys->size());
        if (open) {
            for (std::size_t i = 0; i < p.Keys->size(); i++) {
                if (EditorTreeNode(("#" + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                    MapTypeEditor keyEditor(*this, i, true);
                    p.Keys->accept(keyEditor);
                    MapTypeEditor valueEditor(*this, i, false);
                    p.Values->accept(valueEditor);
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::MulticastSparseDelegateProperty& p) {
    EditorList("Value.InvocationList", p.Value.InvocationList, [&](std::size_t idx, auto& item) {
        if (EditorTreeNode(("#" + std::to_string(idx)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            changed_ |= EditorObjectReference("Object", item.Object, ctx_);
            changed_ |= EditorName("FunctionName", item.FunctionName);
            ImGui::TreePop();
        }
    });
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::NameProperty& p) {
    changed_ |= EditorName("Value", p.Value);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::ObjectProperty& p) {
    changed_ |= EditorObjectReference("Value", p.Value, ctx_);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::SetProperty& p) {
    ImGui::BeginDisabled();
    EditorShowText("SetType", p.SetType().toString().c_str());
    ImGui::EndDisabled();
    SetEditor editor(*this);
    p.Value->accept(editor);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::SoftObjectProperty& p) {
    changed_ |= EditorSoftObjectPath("Value", p.Value);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::StrProperty& p) {
    changed_ |= EditorString("Value", p.Value);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::StructProperty& p) {
    ImGui::BeginDisabled();
    EditorShowText("StructName", p.StructName().toString().c_str());
    EditorShowText("StructGuid", p.StructGuid().toString().c_str());
    ImGui::EndDisabled();
    StructEditor editor(*this);
    p.Value->accept(editor);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::TextProperty& p) {
    EditorShowText("Value", p.Value.string().c_str()); // TODO
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::UInt32Property& p) {
    static constexpr uint32_t step = 1;
    static constexpr uint32_t step_fast = 100;
    changed_ |= EditorScalar("Value", ImGuiDataType_U32, &p.Value, &step, &step_fast);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::UInt64Property& p) {
    static constexpr uint64_t step = 1;
    static constexpr uint64_t step_fast = 100;
    changed_ |= EditorScalar("Value", ImGuiDataType_U64, &p.Value, &step, &step_fast);
}

void Satisfactory3DMap::UI::PropertyEditor::visit(s::UnknownProperty& p) {
    EditorShowBinData("[UnknownProperty]", p.Value, ctx_);
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::BoolArray& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        // TODO checkbox ?
        parent_.changed_ |= EditorScalar(("#" + std::to_string(idx)).c_str(), ImGuiDataType_S8, &item);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::ByteArray& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorScalar(("#" + std::to_string(idx)).c_str(), ImGuiDataType_S8, &item);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::DoubleArray& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorScalar(("#" + std::to_string(idx)).c_str(), ImGuiDataType_Double, &item);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::EnumArray& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorName(("#" + std::to_string(idx)).c_str(), item);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::FloatArray& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorScalar(("#" + std::to_string(idx)).c_str(), ImGuiDataType_Float, &item);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::Int64Array& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorScalar(("#" + std::to_string(idx)).c_str(), ImGuiDataType_S64, &item);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::Int8Array& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorScalar(("#" + std::to_string(idx)).c_str(), ImGuiDataType_S8, &item);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::IntArray& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorScalar(("#" + std::to_string(idx)).c_str(), ImGuiDataType_S32, &item);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::InterfaceArray& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorObjectReference(("#" + std::to_string(idx)).c_str(), item, parent_.ctx_);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::NameArray& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorName(("#" + std::to_string(idx)).c_str(), item);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::ObjectArray& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorObjectReference(("#" + std::to_string(idx)).c_str(), item, parent_.ctx_);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::SoftObjectArray& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorSoftObjectPath(("#" + std::to_string(idx)).c_str(), item);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::StrArray& a) {
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorString(("#" + std::to_string(idx)).c_str(), item);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::ArrayEditor::visit(s::StructArray& a) {
    ImGui::BeginDisabled();
    // a.Name() ?
    EditorShowText("StructName", a.StructName().toString().c_str());
    EditorShowText("StructGuid", a.StructGuid().toString().c_str());
    ImGui::EndDisabled();
    StructEditor editor(parent_);
    EditorList("Values", a.Values, [&](std::size_t idx, auto& item) {
        if (EditorTreeNode(("#" + std::to_string(idx)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            item->accept(editor);
            ImGui::TreePop();
        }
    });
}

void Satisfactory3DMap::UI::PropertyEditor::MapTypeEditor::visit(s::ByteMapTypeList& m) {
    parent_.changed_ |= EditorScalar(label(), ImGuiDataType_S8, &m.List[idx_]);
}

void Satisfactory3DMap::UI::PropertyEditor::MapTypeEditor::visit(s::EnumMapTypeList& m) {
    parent_.changed_ |= EditorName(label(), m.List[idx_]);
}

void Satisfactory3DMap::UI::PropertyEditor::MapTypeEditor::visit(s::FloatMapTypeList& m) {
    parent_.changed_ |= EditorScalar(label(), ImGuiDataType_Float, &m.List[idx_]);
}

void Satisfactory3DMap::UI::PropertyEditor::MapTypeEditor::visit(s::Int64MapTypeList& m) {
    parent_.changed_ |= EditorScalar(label(), ImGuiDataType_S64, &m.List[idx_]);
}

void Satisfactory3DMap::UI::PropertyEditor::MapTypeEditor::visit(s::IntMapTypeList& m) {
    parent_.changed_ |= EditorScalar(label(), ImGuiDataType_S32, &m.List[idx_]);
}

void Satisfactory3DMap::UI::PropertyEditor::MapTypeEditor::visit(s::NameMapTypeList& m) {
    parent_.changed_ |= EditorName(label(), m.List[idx_]);
}

void Satisfactory3DMap::UI::PropertyEditor::MapTypeEditor::visit(s::ObjectMapTypeList& m) {
    parent_.changed_ |= EditorObjectReference(label(), m.List[idx_], parent_.ctx_);
}

void Satisfactory3DMap::UI::PropertyEditor::MapTypeEditor::visit(s::StructMapTypeList& m) {
    const bool open = EditorTreeNode(label(), ImGuiTreeNodeFlags_DefaultOpen);
    ImGui::TableNextColumn();
    ImGui::TextDisabled("%s", m.getStructName().toString().c_str());
    if (open) {
        StructEditor editor(parent_);
        m.List[idx_]->accept(editor);
        ImGui::TreePop();
    }
}

void Satisfactory3DMap::UI::PropertyEditor::SetEditor::visit(s::ObjectSet& s) {
    EditorList("Values", s.Values, [&](std::size_t idx, auto& item) {
        parent_.changed_ |= EditorObjectReference(("#" + std::to_string(idx)).c_str(), item, parent_.ctx_);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::SetEditor::visit(s::StructSet& s) {
    ImGui::BeginDisabled();
    EditorShowText("StructName", s.getStructName().toString().c_str());
    ImGui::EndDisabled();
    StructEditor editor(parent_);
    EditorList("Values", s.Values, [&](std::size_t idx, auto& item) {
        if (EditorTreeNode(("#" + std::to_string(idx)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            item->accept(editor);
            ImGui::TreePop();
        }
    });
}

void Satisfactory3DMap::UI::PropertyEditor::SetEditor::visit(s::UInt32Set& s) {
    EditorList("Values", s.Values, [&](std::size_t idx, auto& item) {
        static constexpr uint32_t step = 1;
        static constexpr uint32_t step_fast = 100;
        parent_.changed_ |=
            EditorScalar(("#" + std::to_string(idx)).c_str(), ImGuiDataType_U32, &item, &step, &step_fast);
    });
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::BoxStruct& s) {
    if (EditorTreeNode("Data", ImGuiTreeNodeFlags_DefaultOpen)) {
        parent_.changed_ |= EditorVector("Min", s.Data.Min);
        parent_.changed_ |= EditorVector("Max", s.Data.Max);
        parent_.changed_ |= EditorScalar("IsValid", ImGuiDataType_U8, &s.Data.IsValid);
        ImGui::TreePop();
    }
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::ClientIdentityInfoStruct& s) {
    if (EditorTreeNode("Data", ImGuiTreeNodeFlags_DefaultOpen)) {
        parent_.changed_ |= EditorString("OfflineId", s.Data.OfflineId);
        EditorMap(
            "AccountIds", s.Data.AccountIds,
            [&](auto& key) {
                parent_.changed_ |= EditorScalar("Key", ImGuiDataType_U8, &key);
            },
            [&](auto& value) {
                // TODO std::vector<uint8_t>
                std::stringstream stream;
                for (const auto& v : value) {
                    stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(v) << " ";
                }
                EditorShowText("Value", stream.str().c_str());
            });

        ImGui::TreePop();
    }
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::ColorStruct& s) {
    parent_.changed_ |= EditorColor("Data", s.Data);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::DateTimeStruct& s) {
    EditorShowText("Data", s.Data.toString().c_str()); // TODO
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::FluidBoxStruct& s) {
    static constexpr float step = 1.0f;
    static constexpr float step_fast = 100.0f;
    parent_.changed_ |= EditorScalar("Data.Value", ImGuiDataType_Float, &s.Data.Value, &step, &step_fast);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::GuidStruct& s) {
    EditorShowText("Data", s.Data.toString().c_str()); // TODO
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::IntPointStruct& s) {
    parent_.changed_ |= EditorIntPoint("Data", s.Data);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::IntVectorStruct& s) {
    parent_.changed_ |= EditorIntVector("Data", s.Data);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::InventoryItemStruct& s) {
    parent_.changed_ |= EditorInventoryItem("Data", s.Data, parent_.ctx_);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::LBBalancerIndexingStruct& s) {
    if (EditorTreeNode("Data", ImGuiTreeNodeFlags_DefaultOpen)) {
        static constexpr int32_t step = 1;
        static constexpr int32_t step_fast = 100;
        parent_.changed_ |= EditorScalar("mNormalIndex", ImGuiDataType_S32, &s.Data.mNormalIndex, &step, &step_fast);
        parent_.changed_ |=
            EditorScalar("mOverflowIndex", ImGuiDataType_S32, &s.Data.mOverflowIndex, &step, &step_fast);
        parent_.changed_ |= EditorScalar("mFilterIndex", ImGuiDataType_S32, &s.Data.mFilterIndex, &step, &step_fast);
        ImGui::TreePop();
    }
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::LinearColorStruct& s) {
    parent_.changed_ |= EditorLinearColor("Data", s.Data);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::PropertyStruct& s) {
    parent_.changed_ |= EditorPropertyList("Data", s.Data, parent_.ctx_);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::QuatStruct& s) {
    parent_.changed_ |= EditorQuat("Data", s.Data);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::RailroadTrackPositionStruct& s) {
    if (EditorTreeNode("Data", ImGuiTreeNodeFlags_DefaultOpen)) {
        parent_.changed_ |= EditorObjectReference("Track", s.Data.Track, parent_.ctx_);
        parent_.changed_ |= EditorScalar("Offset", ImGuiDataType_Float, &s.Data.Offset);
        parent_.changed_ |= EditorScalar("Forward", ImGuiDataType_Float, &s.Data.Forward);
        ImGui::TreePop();
    }
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::RotatorStruct& s) {
    parent_.changed_ |= EditorRotator("Data", s.Data);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::ScalarMaterialInputStruct& s) {
    if (EditorTreeNode("Data", ImGuiTreeNodeFlags_DefaultOpen)) {
        parent_.changed_ |= EditorScalar("OutputIndex", ImGuiDataType_S32, &s.Data.OutputIndex);
        parent_.changed_ |= EditorName("InputName", s.Data.InputName);
        parent_.changed_ |= EditorName("ExpressionName", s.Data.ExpressionName);
        ImGui::TreePop();
    }
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::SoftClassPathStruct& s) {
    parent_.changed_ |= EditorSoftObjectPath("Data", s.Data);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::VectorMaterialInputStruct& s) {
    if (EditorTreeNode("Data", ImGuiTreeNodeFlags_DefaultOpen)) {
        parent_.changed_ |= EditorScalar("OutputIndex", ImGuiDataType_S32, &s.Data.OutputIndex);
        parent_.changed_ |= EditorName("InputName", s.Data.InputName);
        parent_.changed_ |= EditorName("ExpressionName", s.Data.ExpressionName);
        ImGui::TreePop();
    }
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::Vector2DStruct& s) {
    parent_.changed_ |= EditorVector2D("Data", s.Data);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::Vector4Struct& s) {
    parent_.changed_ |= EditorVector4("Data", s.Data);
}

void Satisfactory3DMap::UI::PropertyEditor::StructEditor::visit(s::VectorStruct& s) {
    parent_.changed_ |= EditorVector("Data", s.Data);
}
