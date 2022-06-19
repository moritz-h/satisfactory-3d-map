#include "SettingsWindow.h"

#include <imgui.h>

#include "SettingVisitor.h"
#include "Utils/FileDialogUtil.h"

namespace {
    class SettingsRenderer : public Satisfactory3DMap::SettingVisitor {
        void visit(Satisfactory3DMap::BoolSetting& s) override {
            bool v = s.getVal();
            ImGui::Checkbox(("##" + s.name()).c_str(), &v);
            s.setVal(v);
        }

        void visit(Satisfactory3DMap::EnumSettingBase& s) override {
            int item = static_cast<int>(s.getIdx());
            ImGui::Combo(("##" + s.name()).c_str(), &item, s.names().data(), s.names().size());
            s.setIdx(static_cast<std::size_t>(item));
        }

        void visit(Satisfactory3DMap::FloatSetting& s) override {
            float v = s.getVal();
            ImGui::SliderFloat(("##" + s.name()).c_str(), &v, 0.0f, 1.0f);
            s.setVal(v);
        }
    };
} // namespace

Satisfactory3DMap::SettingsWindow::SettingsWindow(std::shared_ptr<Configuration> config)
    : config_(std::move(config)),
      show_(false) {}

void Satisfactory3DMap::SettingsWindow::renderGui() {
    if (!show_) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(800.0f, 600.0f), ImGuiCond_Appearing);
    const auto& size = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(size.x * 0.5f, size.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Settings##SettingsWindow", &show_);
    if (ImGui::BeginTable("tableSettings", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingFixedFit)) {
        ImGui::TableSetupColumn("Setting");
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        // Game directory
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Game Directory");
        ImGui::TableNextColumn();
        if (ImGui::Button("Select")) {
            auto dir = selectFolder("Select Satisfactory directory ...");
            if (dir.has_value()) {
                config_->setGameDirectory(dir.value());
            }
        }
        ImGui::SameLine();
        ImGui::Text("\"%s\"", config_->getGameDirectory().string().c_str());
        ImGui::Text("(change requires restart)");

        // Registered settings
        SettingsRenderer r;
        for (const auto& setting : config_->getSettings()) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%s", setting->name().c_str());
            ImGui::TableNextColumn();
            setting->accept(r);
        }

        ImGui::EndTable();
    }
    ImGui::End();
}
