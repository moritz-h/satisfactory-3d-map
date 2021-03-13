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

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    auto boxResource = Satisfactory3DMap::getBinaryResource("models/box.glb");
    bool ret = loader.LoadBinaryFromMemory(
        &model, &err, &warn, reinterpret_cast<const unsigned char*>(boxResource.data()), boxResource.size());
    if (!ret || !err.empty() || !warn.empty()) {
        std::cerr << err << std::endl;
        std::cerr << warn << std::endl;
        throw std::runtime_error("Error loading model!");
    }

    std::vector<float> position(model.bufferViews[0].byteLength / sizeof(float));
    std::vector<float> normal(model.bufferViews[1].byteLength / sizeof(float));
    std::vector<float> texcoord(model.bufferViews[2].byteLength / sizeof(float));
    std::vector<unsigned short> boxIndices(model.bufferViews[3].byteLength / sizeof(unsigned short));

    unsigned char* buffer = model.buffers[0].data.data();
    std::copy(buffer + model.bufferViews[0].byteOffset,
        buffer + model.bufferViews[0].byteOffset + model.bufferViews[0].byteLength,
        reinterpret_cast<unsigned char*>(position.data()));
    std::copy(buffer + model.bufferViews[1].byteOffset,
        buffer + model.bufferViews[1].byteOffset + model.bufferViews[1].byteLength,
        reinterpret_cast<unsigned char*>(normal.data()));
    std::copy(buffer + model.bufferViews[2].byteOffset,
        buffer + model.bufferViews[2].byteOffset + model.bufferViews[2].byteLength,
        reinterpret_cast<unsigned char*>(texcoord.data()));
    std::copy(buffer + model.bufferViews[3].byteOffset,
        buffer + model.bufferViews[3].byteOffset + model.bufferViews[3].byteLength,
        reinterpret_cast<unsigned char*>(boxIndices.data()));

    meshBox_ = std::make_unique<glowl::Mesh>(std::vector<std::vector<float>>{position, normal, texcoord}, boxIndices,
        std::vector<glowl::VertexLayout>{
            {12, {{3, GL_FLOAT, GL_FALSE, 0}}}, {12, {{3, GL_FLOAT, GL_FALSE, 0}}}, {8, {{2, GL_FLOAT, GL_FALSE, 0}}}},
        GL_UNSIGNED_SHORT);

    glowl::TextureLayout layout(GL_RGBA8, model.images[0].width, model.images[0].height, 1, GL_RGBA, GL_UNSIGNED_BYTE,
        1,
        {{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE}, {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
            {GL_TEXTURE_MIN_FILTER, GL_LINEAR}, {GL_TEXTURE_MAG_FILTER, GL_LINEAR}},
        {});
    texBox_ = std::make_unique<glowl::Texture2D>("box", layout, model.images[0].image.data());

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
    texBox_->bindTexture();
    shaderBox_->setUniform("tex", 0);

    meshBox_->draw();

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}
