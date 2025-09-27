#include "AssetObjectEditor.h"

#include <imgui.h>

#include "ObjectWidgets.h"

void Satisfactory3DMap::UI::AssetObjectEditor::renderGui() {
    if (assetExport_->Object != nullptr) {
        AssetUObjectEditor e(*this);
        e.dispatch(*assetExport_->Object);
    }
    if (!assetExport_->BinaryClassData.empty()) {
        EditorSectionWrap(0, "Binary Class Data", [&]() {
            EditorShowBinData("Unknown Class Data", assetExport_->BinaryClassData, ctx_);
        });
    }
}

void Satisfactory3DMap::UI::AssetObjectEditor::AssetUObjectEditor::visit(s::UObject& o) {
    EditorSectionWrap(0, "UObject", [&]() {
        EditorPropertyList("Properties", o.Properties, parent_.ctx_);
        EditorOptional("Guid", o.Guid, [&](auto& item) {
            EditorShowText("Value", "TODO FGuid!"); // TODO item
        });
    });
}

void Satisfactory3DMap::UI::AssetObjectEditor::AssetUObjectEditor::visit(s::UStaticMesh& o) {
    visit(static_cast<s::UObject&>(o));
    if (ImGui::CollapsingHeader("UStaticMesh", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::TextUnformatted("TODO.");
    }
}

void Satisfactory3DMap::UI::AssetObjectEditor::AssetUObjectEditor::visit(s::UTexture2D& o) {
    visit(static_cast<s::UObject&>(o));
    if (ImGui::CollapsingHeader("UTexture2D", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (parent_.texture2d_ == nullptr) {
            parent_.texture2d_ = std::make_unique<OGLTexture2D>(o);
        }
        ImVec2 size = ImGui::GetContentRegionAvail();
        size.y =
            size.x / static_cast<float>(parent_.texture2d_->sizeX()) * static_cast<float>(parent_.texture2d_->sizeY());
        ImGui::Image(static_cast<ImTextureID>(parent_.texture2d_->name()), size);
    }
}
