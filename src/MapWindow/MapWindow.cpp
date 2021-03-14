#include "MapWindow.h"

#include <filesystem>

#include <imgui.h>
#include <tiny_gltf.h>

#include "Utils/ResourceUtils.h"

Satisfactory3DMap::MapWindow::MapWindow()
    : BaseWindow("Satisfactory3DMap"), mouseX_(0.0), mouseY_(0.0), cameraControlMode_(Camera::MouseControlMode::None) {

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

void Satisfactory3DMap::MapWindow::openSave(const std::string& filename) {
    if (filename.empty()) {
        return;
    }
    std::filesystem::path filepath(filename);
    if (!std::filesystem::exists(filepath) || !std::filesystem::is_regular_file(filepath)) {
        std::cerr << "No regular file given!" << std::endl;
        return;
    }
    savegame_ = std::make_unique<SaveGame>(filepath);
    savegame_->printHeader();
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
    shaderBox_->setUniform("viewMx", camera_.viewMx());
    shaderBox_->setUniform("modelMx", glm::mat4(1.0f));

    glActiveTexture(GL_TEXTURE0);
    modelBox_->bindTexture();
    shaderBox_->setUniform("tex", 0);

    modelBox_->draw();

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void Satisfactory3DMap::MapWindow::mouseButtonEvent(int button, int action, int mods) {
    cameraControlMode_ = Camera::MouseControlMode::None;
    if (action == GLFW_PRESS && mods == 0) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            cameraControlMode_ = Camera::MouseControlMode::Left;
        } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            cameraControlMode_ = Camera::MouseControlMode::Middle;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            cameraControlMode_ = Camera::MouseControlMode::Right;
        }
    }
}

void Satisfactory3DMap::MapWindow::mouseMoveEvent(double xpos, double ypos) {
    if (cameraControlMode_ != Camera::MouseControlMode::None) {
        double oldX = 2.0 * mouseX_ / static_cast<double>(width_) - 1.0;
        double oldY = 1.0 - 2.0 * mouseY_ / static_cast<double>(height_);
        double newX = 2.0 * xpos / static_cast<double>(width_) - 1.0;
        double newY = 1.0 - 2.0 * ypos / static_cast<double>(height_);
        camera_.mouseMoveControl(cameraControlMode_, oldX, oldY, newX, newY);
    }
    mouseX_ = xpos;
    mouseY_ = ypos;
}

void Satisfactory3DMap::MapWindow::mouseScrollEvent(double xoffset, double yoffset) {
    camera_.mouseScrollControl(xoffset, yoffset);
}

void Satisfactory3DMap::MapWindow::dropEvent(const std::vector<std::string>& paths) {
    if (paths.size() != 1) {
        std::cerr << "Can only read a single file!" << std::endl;
    }
    openSave(paths[0]);
}
