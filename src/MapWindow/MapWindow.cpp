#include "MapWindow.h"

#include <imgui.h>
#include <tiny_gltf.h>

#include "Utils/ResourceUtils.h"

Satisfactory3DMap::MapWindow::MapWindow() : BaseWindow("Satisfactory3DMap") {

    try {
        shaderBox_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/box.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/box.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }

    modelBox_ = std::make_unique<Model>("models/box.glb");

    glEnable(GL_DEPTH_TEST);
}

Satisfactory3DMap::MapWindow::~MapWindow() {
    glDisable(GL_DEPTH_TEST);
}

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

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float aspect = static_cast<float>(width_) / static_cast<float>(height_);
    shaderBox_->use();
    shaderBox_->setUniform("projMx", glm::perspective(glm::radians(45.0f), aspect, 0.01f, 100.0f));
    shaderBox_->setUniform("viewMx", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)));
    shaderBox_->setUniform("modelMx", glm::mat4(1.0f));

    glActiveTexture(GL_TEXTURE0);
    modelBox_->bindTexture();
    shaderBox_->setUniform("tex", 0);

    modelBox_->draw();

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}
