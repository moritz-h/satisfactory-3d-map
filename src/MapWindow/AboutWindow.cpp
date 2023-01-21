#include "AboutWindow.h"

#include <sstream>

#include <imgui.h>
#include <spdlog/spdlog.h>

#include "Utils/LinkUtil.h"
#include "Utils/ResourceUtils.h"

namespace {
    void parseCopyright(std::map<std::string, std::string>& licenseMap, const std::string& prefix) {
        std::stringstream stream(Satisfactory3DMap::getStringResource("copyright/" + prefix + ".list"));
        std::string line;
        int i = 0;
        while (std::getline(stream, line)) {
            if (!line.empty() && line[line.size() - 1] == '\r') {
                line.erase(line.size() - 1);
            }
            const auto license =
                Satisfactory3DMap::getStringResource("copyright/" + prefix + "_" + std::to_string(i) + ".txt");
            licenseMap[line] = license;
            i++;
        }
    }
} // namespace

Satisfactory3DMap::AboutWindow::AboutWindow() : show_(false) {
    try {
        parseCopyright(libraryLicenseMap_, "library");
        parseCopyright(resourceLicenseMap_, "resource");
    } catch (const std::exception& ex) {
        spdlog::error("Error initializing about window: {}", ex.what());
    }
}

void Satisfactory3DMap::AboutWindow::renderGui() {
    if (!show_) {
        return;
    }

    static const std::string versionFull = getStringResource("build_info/VERSION_FULL");
    static const std::string gitVersion = getStringResource("build_info/GIT_VERSION");
    static const std::string licenseText = getStringResource("build_info/LICENSE");
    static const std::string githubUrl = "https://github.com/moritz-h/satisfactory-3d-map";

    ImGui::SetNextWindowSize(ImVec2(400.0f, 400.0f), ImGuiCond_Appearing);
    const auto& size = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(size.x * 0.5f, size.y * 0.5f), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::Begin("About", &show_);
    ImGui::Text("Satisfactory 3D Map");
    ImGui::Text("Version: %s (Git: %s)", versionFull.c_str(), gitVersion.c_str());
    ImGui::Text("");
    ImGui::Text("Copyright (C) 2021 - 2023 Moritz Heinemann");
    ImGui::Text("Homepage:");
    ImGui::SameLine();
    if (ImGui::SmallButton(githubUrl.c_str())) {
        openLink(githubUrl);
    }
    if (ImGui::TreeNode("License")) {
        ImGui::Text("%s", licenseText.c_str());
        ImGui::TreePop();
    }
    ImGui::Text("");
    ImGui::Separator();

    ImGui::Text("Included third-party software:");
    for (const auto& entry : libraryLicenseMap_) {
        if (ImGui::CollapsingHeader(entry.first.c_str(), ImGuiTreeNodeFlags_None)) {
            ImGui::Text("%s", entry.second.c_str());
        }
    }
    ImGui::Separator();
    ImGui::Text("Included resources:");
    for (const auto& entry : resourceLicenseMap_) {
        if (ImGui::CollapsingHeader(entry.first.c_str(), ImGuiTreeNodeFlags_None)) {
            ImGui::Text("%s", entry.second.c_str());
        }
    }

    ImGui::End();
}
