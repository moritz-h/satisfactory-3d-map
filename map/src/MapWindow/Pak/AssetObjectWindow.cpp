#include "AssetObjectWindow.h"

#include <imgui.h>
#include <imgui_memory_editor.h>

#include "AssetWindow.h"

Satisfactory3DMap::AssetObjectWindow::AssetObjectWindow(std::shared_ptr<AssetWindow> assetWindow,
    std::unique_ptr<AssetExport> assetExport)
    : assetWindow_(std::move(assetWindow)),
      assetExport_(std::move(assetExport)) {
    windowTitle_ = "Asset Object View###" + std::to_string(reinterpret_cast<uintptr_t>(this));
}

void Satisfactory3DMap::AssetObjectWindow::renderGui() {
    ImGui::PushID(this);
    ImGui::SetNextWindowSize(ImVec2(500.0f, 600.0f), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(1000.0f, 100.0f), ImGuiCond_Once);
    bool open = true;
    ImGui::Begin(windowTitle_.c_str(), &open);
    if (ImGui::CollapsingHeader("Properties")) {
        if (assetExport_->propertiesError.empty()) {
            propertyRenderer_.renderGui(assetExport_->properties, [&]([[maybe_unused]] const std::string& p) {});
        } else {
            ImGui::Text("Error parsing properties:");
            ImGui::Text("%s", assetExport_->propertiesError.c_str());
        }
    }
    if (ImGui::CollapsingHeader("Hex")) {
        const char* items[] = {"Full", "Properties", "After Prop."};
        static int item_current = 0;
        ImGui::Combo("Show Hex", &item_current, items, IM_ARRAYSIZE(items));
        auto& bin = assetExport_->binary;
        const auto& propBinSize = assetExport_->propertiesBinSize;
        void* data = bin.data();
        std::size_t size = bin.size();
        if (item_current == 1) {
            data = bin.data();
            size = propBinSize;
        } else if (item_current == 2) {
            data = bin.data() + propBinSize;
            size = bin.size() - propBinSize;
        }
        static MemoryEditor hexEditor;
        hexEditor.ReadOnly = true;
        hexEditor.DrawContents(data, size);
    }
    ImGui::End();
    ImGui::PopID();
    if (!open) {
        assetWindow_->queueCloseExportWindow(shared_from_this());
    }
}
