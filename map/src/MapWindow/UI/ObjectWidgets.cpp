#include "ObjectWidgets.h"

#include <array>

#include <IconsFontAwesome6.h>
#include <imgui_stdlib.h>

#include "PropertyEditor.h"

void Satisfactory3DMap::UI::EndEditorTable() {
    ImGui::PopID();
    ImGui::EndTable();
}

void Satisfactory3DMap::UI::PushEditorTableStyle() {
    // Relative scaling of frame padding for DPI scaling.
    ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding.y * 2.0f / 3.0f);
    ImGui::PushStyleVarX(ImGuiStyleVar_CellPadding, ImGui::GetStyle().CellPadding.x * 0.5f);
}

void Satisfactory3DMap::UI::PopEditorTableStyle() {
    ImGui::PopStyleVar(2);
}

bool Satisfactory3DMap::UI::TreeNodeSmall(const char* label, ImGuiTreeNodeFlags flags) {
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    bool is_open = ImGui::TreeNodeEx(label, flags);
    ImGui::PopStyleVar();
    return is_open;
}

bool Satisfactory3DMap::UI::EditorTreeNode(const char* label, ImGuiTreeNodeFlags flags) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    return TreeNodeSmall(label, flags | ImGuiTreeNodeFlags_SpanFullWidth);
}

bool Satisfactory3DMap::UI::EditorTreeStartLeaf(const char* label, ImGuiTreeNodeFlags flags) {
    ImGui::TableNextRow();
    ImGui::PushID(label);
    ImGui::TableNextColumn();
    // TODO: AlignTextToFramePadding centers text to text of widgets, but background frame of tree node is not aligned.
    // ImGui::AlignTextToFramePadding();
    return TreeNodeSmall(label, flags | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf |
                                    ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
}

void Satisfactory3DMap::UI::EditorTreeEndLeaf() {
    ImGui::PopID();
}

void Satisfactory3DMap::UI::ClassOrPathButton(const std::string& name, const EventContext& ctx) {
    if (name.starts_with('/')) {
        if (ctx.selectClass) {
            ImGui::PushStyleVarX(ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding.x * 0.5f);
            if (ImGui::SmallButton(ICON_FA_UP_RIGHT_FROM_SQUARE)) {
                ctx.selectClass(name);
            }
            ImGui::PopStyleVar();
        }
    } else if (!name.empty()) {
        if (ctx.selectPath) {
            ImGui::PushStyleVarX(ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding.x * 0.5f);
            if (ImGui::SmallButton(ICON_FA_HAND_POINTER)) {
                ctx.selectPath(name);
            }
            ImGui::PopStyleVar();
        }
    }
}

void Satisfactory3DMap::UI::EditorShowSelectable(const char* label, const std::string& name, const EventContext& ctx) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::TextUnformatted(name.c_str());
    if (ImGui::BeginPopupContextItem(label)) {
        if (ImGui::Selectable(ICON_FA_COPY " Copy")) {
            ImGui::SetClipboardText(name.c_str());
        }
        ImGui::EndPopup();
    }
    ImGui::TableNextColumn();
    ClassOrPathButton(name, ctx);
    EditorTreeEndLeaf();
}

void Satisfactory3DMap::UI::EditorShowText(const char* label, const char* text, bool disabled) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    if (disabled) {
        ImGui::TextDisabled("%s", text);
    } else {
        ImGui::TextUnformatted(text);
    }
    EditorTreeEndLeaf();
}

void Satisfactory3DMap::UI::EditorShowBinData(const char* label, const std::vector<char>& data,
    const EventContext& ctx) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::PushStyleColor(ImGuiCol_Button, static_cast<ImVec4>(ImColor::HSV(0.0f, 0.6f, 0.6f)));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, static_cast<ImVec4>(ImColor::HSV(0.0f, 0.7f, 0.7f)));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, static_cast<ImVec4>(ImColor::HSV(0.0f, 0.8f, 0.8f)));
    const bool pressedShow = ImGui::SmallButton("Show Hex");
    ImGui::SameLine();
    const bool pressedCopy = ImGui::SmallButton("Copy Hex");
    ImGui::PopStyleColor(3);
    if (pressedShow && ctx.showBinData) {
        ctx.showBinData(data);
    }
    if (pressedCopy) {
        std::stringstream stream;
        for (const auto& c : data) {
            stream << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(static_cast<unsigned char>(c))
                   << " ";
        }
        std::string hex_data = stream.str();
        ImGui::SetClipboardText(hex_data.c_str());
    }
    ImGui::SameLine();
    ImGui::Text("size: %zu", data.size());
    EditorTreeEndLeaf();
}

bool Satisfactory3DMap::UI::EditorBool(const char* label, bool& v) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    const bool changed = ImGui::Checkbox("##bool", &v);
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step,
    const void* p_step_fast, const char* format, ImGuiInputTextFlags flags) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    const bool changed = ImGui::InputScalar("##scalar", data_type, p_data, p_step, p_step_fast, format, flags);
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorString(const char* label, std::string& str) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    const bool changed = ImGui::InputText("##string", &str);
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorName(const char* label, s::FName& name) {
    bool changed = false;
    if (EditorTreeNode(label, ImGuiTreeNodeFlags_DefaultOpen)) {
        EditorTreeStartLeaf("Name");
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(-FLT_MIN);
        changed |= ImGui::InputText("##Name", &name.Name);
        EditorTreeEndLeaf();
        changed |= EditorScalar("Number", ImGuiDataType_U32, &name.Number);
        ImGui::TreePop();
    }
    return changed;
}

bool Satisfactory3DMap::UI::EditorObjectReference(const char* label, s::FObjectReferenceDisc& r,
    const EventContext& ctx) {
    bool changed = false;
    if (EditorTreeNode(label, ImGuiTreeNodeFlags_DefaultOpen)) {
        EditorTreeStartLeaf("Level");
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(-FLT_MIN);
        changed |= ImGui::InputText("##LevelName", &r.LevelName);
        EditorTreeEndLeaf();
        EditorTreeStartLeaf("Path");
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(-FLT_MIN);
        changed |= ImGui::InputText("##PathName", &r.PathName);
        ImGui::TableNextColumn();
        ClassOrPathButton(r.PathName, ctx);
        EditorTreeEndLeaf();
        ImGui::TreePop();
    }
    return changed;
}

bool Satisfactory3DMap::UI::EditorSoftObjectPath(const char* label, s::FSoftObjectPath& p) {
    bool changed = false;
    if (EditorTreeNode(label, ImGuiTreeNodeFlags_DefaultOpen)) {
        changed |= EditorName("AssetPath.PackageName", p.AssetPath.PackageName);
        changed |= EditorName("AssetPath.AssetName", p.AssetPath.AssetName);
        changed |= EditorString("SubPathString", p.SubPathString);
        ImGui::TreePop();
    }
    return changed;
}

bool Satisfactory3DMap::UI::EditorVector(const char* label, s::FVector& v) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    const bool changed = ImGui::InputScalarN("##Vector", ImGuiDataType_Double, reinterpret_cast<double*>(&v), 3);
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorVector2D(const char* label, s::FVector2D& v) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    const bool changed = ImGui::InputScalarN("##Vector2D", ImGuiDataType_Double, reinterpret_cast<double*>(&v), 2);
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorVector4(const char* label, s::FVector4& v) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    const bool changed = ImGui::InputScalarN("##Vector4", ImGuiDataType_Double, reinterpret_cast<double*>(&v), 4);
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorIntVector(const char* label, s::FIntVector& v) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    const bool changed = ImGui::InputScalarN("##IntVector", ImGuiDataType_S32, reinterpret_cast<int32_t*>(&v), 3);
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorIntPoint(const char* label, s::FIntPoint& p) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    const bool changed = ImGui::InputScalarN("##IntPoint", ImGuiDataType_S32, reinterpret_cast<int32_t*>(&p), 2);
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorQuat(const char* label, s::FQuat& q) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    const bool changed = ImGui::InputScalarN("##Quat", ImGuiDataType_Double, reinterpret_cast<double*>(&q), 4);
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorRotator(const char* label, s::FRotator& r) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    const bool changed = ImGui::InputScalarN("##Rotator", ImGuiDataType_Double, reinterpret_cast<double*>(&r), 3);
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorColor(const char* label, s::FColor& c) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    std::array<float, 4> col{
        static_cast<float>(c.R) / 255.0f,
        static_cast<float>(c.G) / 255.0f,
        static_cast<float>(c.B) / 255.0f,
        static_cast<float>(c.A) / 255.0f,
    };
    const bool changed = ImGui::ColorEdit4("##Color", col.data(), ImGuiColorEditFlags_Uint8);
    if (changed) {
        c.R = static_cast<uint8_t>(std::clamp(col[0], 0.0f, 1.0f) * 255.0 + 0.5f);
        c.G = static_cast<uint8_t>(std::clamp(col[1], 0.0f, 1.0f) * 255.0 + 0.5f);
        c.B = static_cast<uint8_t>(std::clamp(col[2], 0.0f, 1.0f) * 255.0 + 0.5f);
        c.A = static_cast<uint8_t>(std::clamp(col[3], 0.0f, 1.0f) * 255.0 + 0.5f);
    }
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorLinearColor(const char* label, s::FLinearColor& c) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    const bool changed = ImGui::ColorEdit4("##LinearColor", reinterpret_cast<float*>(&c), ImGuiColorEditFlags_Float);
    EditorTreeEndLeaf();
    return changed;
}

bool Satisfactory3DMap::UI::EditorInventoryItem(const char* label, s::FInventoryItem& i, const EventContext& ctx) {
    bool changed = false;
    if (EditorTreeNode(label, ImGuiTreeNodeFlags_DefaultOpen)) {
        changed |= EditorObjectReference("ItemClass", i.ItemClass, ctx);
        EditorDynamicStruct("ItemState", i.ItemState, ctx);
        changed |= EditorObjectReference("LegacyItemStateActor", i.LegacyItemStateActor, ctx);
        ImGui::TreePop();
    }
    return changed;
}

bool Satisfactory3DMap::UI::EditorConveyorBeltItem(const char* label, s::FConveyorBeltItem& i,
    const EventContext& ctx) {
    bool changed = false;
    if (EditorTreeNode(label, ImGuiTreeNodeFlags_DefaultOpen)) {
        changed |= EditorInventoryItem("Item", i.Item, ctx);
        changed |= EditorScalar("Offset", ImGuiDataType_Float, &i.Offset);
        ImGui::TreePop();
    }
    return changed;
}

bool Satisfactory3DMap::UI::EditorDynamicStruct(const char* label, s::FFGDynamicStruct& s, const EventContext& ctx) {
    bool changed = false;
    if (EditorTreeNode(label, ImGuiTreeNodeFlags_DefaultOpen)) {
        changed |= EditorObjectReference("ScriptStruct", s.ScriptStruct, ctx);
        changed |= EditorPropertyList("StructInstance", s.StructInstance, ctx);
        ImGui::TreePop();
    }
    return changed;
}

bool Satisfactory3DMap::UI::EditorProperty(s::Property& p, const EventContext& ctx) {
    const bool open = EditorTreeNode(p.Name().toString().c_str(), ImGuiTreeNodeFlags_DefaultOpen);
    ImGui::TableNextColumn();
    ImGui::TextDisabled("%s", p.Type().toString().c_str());
    if (open) {
        PropertyEditor editor(ctx);
        p.accept(editor);
        ImGui::TreePop();
        return editor.changed();
    }
    return false;
}

bool Satisfactory3DMap::UI::EditorPropertyList(const char* label, s::PropertyList& properties,
    const EventContext& ctx) {
    bool changed = false;
    EditorList(label, properties, [&](std::size_t idx, auto& item) {
        ImGui::PushID(static_cast<int>(idx)); // Property name is not unique. Some properties appear duplicated.
        changed |= EditorProperty(*item, ctx);
        ImGui::PopID();
    });
    return changed;
}
