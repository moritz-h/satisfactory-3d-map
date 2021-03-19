#include "MapWindow.h"

#include <filesystem>

#include <glm/gtc/matrix_inverse.hpp>
#include <imgui.h>
#include <tiny_gltf.h>

#include "SaveGame/Objects/SaveActor.h"
#include "Utils/ResourceUtils.h"

namespace {
    void drawObjectTreeGui(const Satisfactory3DMap::SaveGame::SaveNode& n) {
        for (const auto& child : n.childNodes) {
            std::string counts =
                " (A:" + std::to_string(child.second.numActors) + " O:" + std::to_string(child.second.numObjects) + ")";
            if (ImGui::TreeNode((child.first + counts).c_str())) {
                drawObjectTreeGui(child.second);
                ImGui::TreePop();
            }
        }
        for (const auto& obj : n.objects) {
            if (obj.second->type() == 1) {
                ImGui::Text("[A] %s", obj.first.c_str());
            } else {
                ImGui::Text("[O] %s", obj.first.c_str());
            }
        }
    }
} // namespace

Satisfactory3DMap::MapWindow::MapWindow()
    : BaseWindow("Satisfactory3DMap"),
      mouseX_(0.0),
      mouseY_(0.0),
      cameraControlMode_(Camera::MouseControlMode::None),
      camera_(8000.0f),
      numActorsCube_(0),
      numActorsFoundation8x4_(0),
      numActorsFoundation8x2_(0),
      numActorsFoundation8x1_(0) {

    try {
        shaderModels_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/model.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/model.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }

    modelCube_ = std::make_unique<Model>("models/cube.glb");
    modelFoundation8x4_ = std::make_unique<Model>("models/foundation_8x4.glb");
    modelFoundation8x2_ = std::make_unique<Model>("models/foundation_8x2.glb");
    modelFoundation8x1_ = std::make_unique<Model>("models/foundation_8x1.glb");

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

    numActorsCube_ = 0;
    numActorsFoundation8x4_ = 0;
    numActorsFoundation8x2_ = 0;
    numActorsFoundation8x1_ = 0;
    std::vector<float> positionsCube;
    std::vector<float> positionsFoundation8x4;
    std::vector<float> positionsFoundation8x2;
    std::vector<float> positionsFoundation8x1;
    for (const auto& obj : savegame_->saveObjects()) {
        if (obj->type() == 1) {
            const auto* actor = dynamic_cast<SaveActor*>(obj.get());
            const auto& pos = actor->position();
            if (actor->className() ==
                "/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C") {
                positionsFoundation8x4.push_back(pos.x / 100.0f);
                positionsFoundation8x4.push_back(-pos.y / 100.0f);
                positionsFoundation8x4.push_back(pos.z / 100.0f);
                positionsFoundation8x4.push_back(0.0f); // std430 vec4 alignment
                numActorsFoundation8x4_++;
            } else if (actor->className() == "/Game/FactoryGame/Buildable/Building/Foundation/"
                                             "Build_Foundation_8x2_01.Build_Foundation_8x2_01_C") {
                positionsFoundation8x2.push_back(pos.x / 100.0f);
                positionsFoundation8x2.push_back(-pos.y / 100.0f);
                positionsFoundation8x2.push_back(pos.z / 100.0f);
                positionsFoundation8x2.push_back(0.0f); // std430 vec4 alignment
                numActorsFoundation8x2_++;
            } else if (actor->className() == "/Game/FactoryGame/Buildable/Building/Foundation/"
                                             "Build_Foundation_8x1_01.Build_Foundation_8x1_01_C") {
                positionsFoundation8x1.push_back(pos.x / 100.0f);
                positionsFoundation8x1.push_back(-pos.y / 100.0f);
                positionsFoundation8x1.push_back(pos.z / 100.0f);
                positionsFoundation8x1.push_back(0.0f); // std430 vec4 alignment
                numActorsFoundation8x1_++;
            } else {
                positionsCube.push_back(pos.x / 100.0f);
                positionsCube.push_back(-pos.y / 100.0f);
                positionsCube.push_back(pos.z / 100.0f);
                positionsCube.push_back(0.0f); // std430 vec4 alignment
                numActorsCube_++;
            }
        }
    }

    posBufferCube_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, positionsCube);
    posBufferFoundation8x4_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, positionsFoundation8x4);
    posBufferFoundation8x2_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, positionsFoundation8x2);
    posBufferFoundation8x1_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, positionsFoundation8x1);
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

    ImGui::SetNextWindowPos(ImVec2(10.0, 20.0), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("FPS overlay", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoMove);
    ImGui::Text("%.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    ImGui::End();

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (savegame_ == nullptr) {
        return;
    }

    ImGui::SetNextWindowPos(ImVec2(10.0, 60.0), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(200.0, 200.0), ImGuiCond_Once);
    ImGui::Begin(title_.c_str());
    drawObjectTreeGui(savegame_->root());
    ImGui::End();

    float aspect = static_cast<float>(width_) / static_cast<float>(height_);
    shaderModels_->use();
    shaderModels_->setUniform("projMx", glm::perspective(glm::radians(45.0f), aspect, 0.01f, 10000.0f));
    shaderModels_->setUniform("viewMx", camera_.viewMx());
    shaderModels_->setUniform("invViewMx", glm::inverse(camera_.viewMx()));

    glActiveTexture(GL_TEXTURE0);
    shaderModels_->setUniform("tex", 0);

    shaderModels_->setUniform("modelMx", modelCube_->modelMx());
    shaderModels_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelCube_->modelMx())));
    modelCube_->bindTexture();
    posBufferCube_->bind(0);
    modelCube_->draw(numActorsCube_);

    shaderModels_->setUniform("modelMx", modelFoundation8x4_->modelMx());
    shaderModels_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelFoundation8x4_->modelMx())));
    modelFoundation8x4_->bindTexture();
    posBufferFoundation8x4_->bind(0);
    modelFoundation8x4_->draw(numActorsFoundation8x4_);

    shaderModels_->setUniform("modelMx", modelFoundation8x2_->modelMx());
    shaderModels_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelFoundation8x2_->modelMx())));
    modelFoundation8x2_->bindTexture();
    posBufferFoundation8x2_->bind(0);
    modelFoundation8x2_->draw(numActorsFoundation8x2_);

    shaderModels_->setUniform("modelMx", modelFoundation8x1_->modelMx());
    shaderModels_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelFoundation8x1_->modelMx())));
    modelFoundation8x1_->bindTexture();
    posBufferFoundation8x1_->bind(0);
    modelFoundation8x1_->draw(numActorsFoundation8x1_);

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
