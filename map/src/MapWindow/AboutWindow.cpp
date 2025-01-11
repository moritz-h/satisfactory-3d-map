#include "AboutWindow.h"

#include <sstream>

#include <imgui.h>
#include <spdlog/spdlog.h>

#include "Utils/LinkUtil.h"
#include "Utils/ResourceUtils.h"

namespace {
    using namespace Satisfactory3DMap;

    void parseCopyright(std::map<std::string, std::string>& licenseMap) {
        const std::string copyrightDir("copyright");
        for (const auto& entry : resourceDirContent(copyrightDir)) {
            const auto entryDir = copyrightDir + "/" + entry;
            const auto nameFile = entryDir + "/name";
            const auto licenseFile = entryDir + "/copyright";
            std::string name = resourceExists(nameFile) ? getStringResource(nameFile) : "_error_";
            std::string license = resourceExists(licenseFile) ? getStringResource(licenseFile) : "_missing_";
            licenseMap[name] = license;
        }
    }
} // namespace

Satisfactory3DMap::AboutWindow::AboutWindow() : show_(false) {
    try {
        parseCopyright(licenseMap_);
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
    ImGui::TextUnformatted("Satisfactory 3D Map");
    ImGui::Text("Version: %s (Git: %s)", versionFull.c_str(), gitVersion.c_str());
    ImGui::TextUnformatted("");
    ImGui::TextUnformatted("Copyright (C) 2021 - 2024 Moritz Heinemann");
    ImGui::TextUnformatted("Homepage:");
    ImGui::SameLine();
    if (ImGui::SmallButton(githubUrl.c_str())) {
        openLink(githubUrl);
    }
    if (ImGui::TreeNode("License")) {
        ImGui::Text("%s", licenseText.c_str());
        ImGui::TreePop();
    }
    ImGui::TextUnformatted("");
    ImGui::Separator();

    ImGui::TextUnformatted("Third-party software and resources:");
    for (const auto& entry : licenseMap_) {
        if (ImGui::CollapsingHeader(entry.first.c_str(), ImGuiTreeNodeFlags_None)) {
            ImGui::Text("%s", entry.second.c_str());
        }
    }

    ImGui::End();
}
