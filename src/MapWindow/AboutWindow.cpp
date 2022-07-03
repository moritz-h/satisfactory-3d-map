#include "AboutWindow.h"

#include <sstream>

#include <imgui.h>
#include <spdlog/spdlog.h>

#include "Utils/ResourceUtils.h"

Satisfactory3DMap::AboutWindow::AboutWindow() : show_(false) {
    try {
        std::stringstream stream(getStringResource("licenses/licenses.list"));
        std::string line;
        int i = 0;
        while (std::getline(stream, line)) {
            if (!line.empty() && line[line.size() - 1] == '\r') {
                line.erase(line.size() - 1);
            }
            const auto license = getStringResource("licenses/library_" + std::to_string(i) + ".txt");
            libraryLicenseMap_[line] = license;
            i++;
        }
    } catch (const std::exception& ex) {
        spdlog::error("Error initializing about window: {}", ex.what());
    }
}

void Satisfactory3DMap::AboutWindow::renderGui() {
    if (!show_) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f), ImGuiCond_Appearing);
    const auto& size = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(size.x * 0.5f, size.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::Begin("About", &show_);
    ImGui::Text("Satisfactory 3D Map");
    ImGui::Separator();

    ImGui::Text("Included third-party software:");
    for (const auto& entry : libraryLicenseMap_) {
        if (ImGui::CollapsingHeader(entry.first.c_str(), ImGuiTreeNodeFlags_None)) {
            ImGui::Text("%s", entry.second.c_str());
        }
    }

    ImGui::End();
}
