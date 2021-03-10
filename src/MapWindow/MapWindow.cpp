#include "MapWindow.h"

#include <imgui.h>

Satisfactory3DMap::MapWindow::MapWindow() : BaseWindow("Satisfactory3DMap") {}

Satisfactory3DMap::MapWindow::~MapWindow() {}

void Satisfactory3DMap::MapWindow::render() {
    ImGui::DockSpaceOverViewport(
        nullptr, ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open")) {}
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    ImGui::SetNextWindowPos(ImVec2(10.0, 40.0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(200.0, 200.0), ImGuiCond_Once);

    ImGui::Begin(title_.c_str());
    ImGui::Text("Hello imgui!");
    ImGui::End();
}
