#include "ObjectWidgets.h"

#include <imgui_stdlib.h>

namespace {
    void ContextMenuCopySelect(const char* str_id, const std::string& name,
        const Satisfactory3DMap::UI::StringCallback& callback) {
        if (ImGui::BeginPopupContextItem(str_id)) {
            if (ImGui::Selectable(ICON_FA_COPY " Copy")) {
                ImGui::SetClipboardText(name.c_str());
            }
            if (callback && ImGui::Selectable(ICON_FA_UP_RIGHT_FROM_SQUARE " Select")) {
                callback(name);
            }
            ImGui::EndPopup();
        }
    }
} // namespace

void Satisfactory3DMap::UI::ObjectReference(const char* label, const s::FObjectReferenceDisc& r,
    const StringCallback& callback) {
    if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Level: %s", r.LevelName.c_str());
        ImGui::Text("Path: %s", r.PathName.c_str());
        ContextMenuCopySelect(label, r.PathName, callback);
        ImGui::TreePop();
    }
}

bool Satisfactory3DMap::UI::InputObjectReference(const char* label, s::FObjectReferenceDisc& r,
    const StringCallback& callback) {
    bool changed = false;
    if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen)) {
        changed |= ImGui::InputText("Level", &r.LevelName);
        changed |= ImGui::InputText("Path", &r.PathName);
        if (callback) {
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UP_RIGHT_FROM_SQUARE)) {
                callback(r.PathName);
            }
        }
        ImGui::TreePop();
    }
    return changed;
}

void Satisfactory3DMap::UI::ClassName(const char* label, const std::string& className, const StringCallback& callback) {
    // Ignore id part after "##".
    const char* label_end = std::strstr(label, "##");
    int label_length =
        (label_end != nullptr) ? static_cast<int>(label_end - label) : static_cast<int>(std::strlen(label));
    ImGui::Text("%.*s %s", label_length, label, className.c_str());
    ContextMenuCopySelect(label, className, callback);
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
