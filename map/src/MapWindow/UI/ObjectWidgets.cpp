#include "ObjectWidgets.h"

#include <imgui_stdlib.h>

void Satisfactory3DMap::UI::ClassOrPathButton(const std::string& name, const SelectionContext& ctx) {
    if (name.starts_with('/')) {
        if (ctx.classCallback) {
            ImGui::SameLine();
            if (ImGui::SmallButton(ICON_FA_UP_RIGHT_FROM_SQUARE " Find Class")) {
                ctx.classCallback(name);
            }
        }
    } else if (!name.empty()) {
        if (ctx.pathCallback) {
            ImGui::SameLine();
            if (ImGui::SmallButton(ICON_FA_UP_RIGHT_FROM_SQUARE " Select")) {
                ctx.pathCallback(name);
            }
        }
    }
}

void Satisfactory3DMap::UI::SelectableName(const char* label, const std::string& name, const SelectionContext& ctx) {
    // Ignore id part after "##".
    const char* label_end = std::strstr(label, "##");
    int label_length =
        (label_end != nullptr) ? static_cast<int>(label_end - label) : static_cast<int>(std::strlen(label));
    ImGui::Text("%.*s %s", label_length, label, name.c_str());
    if (ImGui::BeginPopupContextItem(label)) {
        if (ImGui::Selectable(ICON_FA_COPY " Copy")) {
            ImGui::SetClipboardText(name.c_str());
        }
        ImGui::EndPopup();
    }
    ClassOrPathButton(name, ctx);
}

void Satisfactory3DMap::UI::ObjectReference(const char* label, const s::FObjectReferenceDisc& r,
    const SelectionContext& ctx) {
    if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Level: %s", r.LevelName.c_str());
        UI::SelectableName("Path:", r.PathName, ctx);
        ImGui::TreePop();
    }
}

bool Satisfactory3DMap::UI::InputObjectReference(const char* label, s::FObjectReferenceDisc& r,
    const SelectionContext& ctx) {
    bool changed = false;
    if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen)) {
        changed |= ImGui::InputText("Level", &r.LevelName);
        changed |= ImGui::InputText("Path", &r.PathName);
        ClassOrPathButton(r.PathName, ctx);
        ImGui::TreePop();
    }
    return changed;
}

void Satisfactory3DMap::UI::LinearColor(const char* label, const s::FLinearColor& c) {
    ImGui::BulletText("%s: R:%f G:%f B:%f A:%f", label, c.R, c.G, c.B, c.A);
}

bool Satisfactory3DMap::UI::InputLinearColor(const char* label, s::FLinearColor& c) {
    bool changed = false;
    if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen)) {
        changed |= ImGui::ColorEdit4("##color", reinterpret_cast<float*>(&c), ImGuiColorEditFlags_Float);
        ImGui::TreePop();
    }
    return changed;
}
