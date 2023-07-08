#include "SettingsWindow.h"

#include <imgui.h>
#include <imgui_stdlib.h>

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
            ImGui::Combo(("##" + s.name()).c_str(), &item, s.names().data(), static_cast<int>(s.names().size()));
            s.setIdx(static_cast<std::size_t>(item));
        }

        void visit(Satisfactory3DMap::FloatSetting& s) override {
            float v = s.getVal();
            ImGui::SliderFloat(("##" + s.name()).c_str(), &v, 0.0f, 1.0f);
            s.setVal(v);
        }

        void visit(Satisfactory3DMap::PathSetting& s) override {
            std::filesystem::path v = s.getVal();

            if (ImGui::Button("Select")) {
                std::optional<std::filesystem::path> result;
                if (s.getType() == Satisfactory3DMap::PathSetting::PathType::Directory) {
                    result = Satisfactory3DMap::selectFolder("Select directory ...");
                } else {
                    result = Satisfactory3DMap::openFile("Select file ...");
                }
                if (result.has_value()) {
                    v = result.value();
                }
            }

            ImGui::SameLine();
            if (!v.empty()) {
                ImGui::Text("%s", v.string().c_str());
            } else {
                ImGui::Text("None");
            }

            s.setVal(v);
        }

        void visit(Satisfactory3DMap::StringSetting& s) override {
            std::string v = s.getVal();
            ImGui::InputText(("##" + s.name()).c_str(), &v);
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

        // Registered settings
        SettingsRenderer r;
        for (const auto& setting : config_->getSettings()) {
            if (setting->isHidden()) {
                continue;
            }
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%s", setting->name().c_str());
            ImGui::TableNextColumn();
            setting->accept(r);
        }

        ImGui::EndTable();
    }
    if (ImGui::Button("Reset to default")) {
        for (const auto& setting : config_->getSettings()) {
            setting->resetDefault();
        }
    }
    ImGui::End();
}
