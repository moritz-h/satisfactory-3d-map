#include "ObjectWidgets.h"

#include <IconsFontAwesome6.h>
#include <imgui_stdlib.h>

bool Satisfactory3DMap::UI::BeginEditorTable() {
    if (ImGui::BeginTable("##PropertyTable", 3, ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 1.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 2.0f);
        ImGui::TableSetupColumn("Button", ImGuiTableColumnFlags_WidthFixed, 15.0f);
        return true;
    }
    return false;
}

void Satisfactory3DMap::UI::EndEditorTable() {
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

void Satisfactory3DMap::UI::EditorShowText(const char* label, const char* text) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::TextUnformatted(text);
    EditorTreeEndLeaf();
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

bool Satisfactory3DMap::UI::EditorLinearColor(const char* label, s::FLinearColor& c) {
    EditorTreeStartLeaf(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-FLT_MIN);
    const bool changed = ImGui::ColorEdit4("##color", reinterpret_cast<float*>(&c), ImGuiColorEditFlags_Float);
    EditorTreeEndLeaf();
    return changed;
}
