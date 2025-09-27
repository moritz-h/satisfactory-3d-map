#include "AssetObjectWindow.h"

#include "../UI/ObjectWidgets.h"
#include "AssetWindow.h"

Satisfactory3DMap::AssetObjectWindow::AssetObjectWindow(std::shared_ptr<AssetWindow> assetWindow,
    std::shared_ptr<s::AssetExport> assetExport, const std::string& title)
    : assetWindow_(std::move(assetWindow)),
      assetExport_(std::move(assetExport)),
      objEditor_(assetExport_, ctx_),
      showHex_(false) {
    windowTitle_ = title + "###" + std::to_string(reinterpret_cast<uintptr_t>(this));
    ctx_.showBinData = [this](const std::vector<char>&) { showHex_ = !showHex_; };
    hexEditor_.ReadOnly = true;
}

void Satisfactory3DMap::AssetObjectWindow::renderGui() {
    ImGui::PushID(this);
    ImGui::SetNextWindowSize(ImVec2(500.0f, 600.0f), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(1000.0f, 100.0f), ImGuiCond_Once);
    bool open = true;
    ImGui::Begin(windowTitle_.c_str(), &open);
    objEditor_.renderGui();
    if (showHex_) {
        if (ImGui::CollapsingHeader("Binary Class Data Hex", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto& bin = assetExport_->BinaryClassData;
            hexEditor_.DrawContents(bin.data(), bin.size());
        }
    }
    ImGui::End();
    ImGui::PopID();
    if (!open) {
        assetWindow_->queueCloseExportWindow(shared_from_this());
    }
}
