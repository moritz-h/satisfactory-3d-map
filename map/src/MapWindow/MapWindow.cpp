#include "MapWindow.h"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <stdexcept>

#include <glm/gtc/matrix_inverse.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_memory_editor.h>
#include <spdlog/spdlog.h>

#include "SatisfactorySave/GameTypes/Save/SaveObject.h"
#include "SatisfactorySave/GameTypes/UE/Engine/GameFramework/Actor.h"
#include "SatisfactorySave/Pak/PakFile.h"
#include "SatisfactorySave/Utils/SaveTextExporter.h"

#include "Camera/Camera3D.h"
#include "UI/ObjectEditor.h"
#include "UI/ObjectWidgets.h"
#include "Utils/FileDialogUtil.h"
#include "Utils/GLMUtil.h"
#include "Utils/ResourceUtils.h"

Satisfactory3DMap::MapWindow::MapWindow()
    : BaseWindow("Satisfactory3DMap"),
      mapViewLeft_(0),
      mapViewTop_(0),
      mapViewWidth_(-1),
      mapViewHeight_(-1),
      lastTickTime_(0.0),
      mapActive_(false),
      keyDownForward_(false),
      keyDownBackward_(false),
      keyDownLeft_(false),
      keyDownRight_(false),
      keyDownUp_(false),
      keyDownDown_(false),
      keyDownTurnLeft_(false),
      keyDownTurnRight_(false),
      keyDownTurnUp_(false),
      keyDownTurnDown_(false),
      mouseX_(0.0),
      mouseY_(0.0),
      mouseHidden_(false),
      mouseMoved_(false),
      cameraControlMode_(AbstractCamera::MouseControlMode::None),
      camera_(std::make_unique<Camera3D>()),
      projMx_(glm::mat4(1.0f)),
      showSaveHeader_(false),
      showCameraWindow_(false),
      showHexEdit_(false) {

    samplingFactorSetting_ =
        EnumSetting<float>::create("SuperSampling", {"0.25x", "0.5x", "1x", "1.5x", "2x", "2.5x", "3x", "4x"},
            std::vector<float>{0.25f, 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 4.0f}, 4);
    metallicSetting_ = FloatSetting::create("Metallic", 0.0f);
    roughnessSetting_ = FloatSetting::create("Roughness", 0.5f);
    worldRenderModeSetting_ = EnumSetting<WorldRenderMode>::create("World Mode",
        {"None", "HeightMap",
#ifdef FEATURE_TILEMAP
            "TileMap"
#else
            "HeightMap (temp. replacement for TileMap)"
#endif
        },
        {WorldRenderMode::None, WorldRenderMode::HeightMap,
#ifdef FEATURE_TILEMAP
            WorldRenderMode::TileMap
#else
            WorldRenderMode::HeightMap
#endif
        },
        2);
    showSelectionMarkerSetting_ = BoolSetting::create("Selection marker", false);
    showSaveTreePerLevelSetting_ = BoolSetting::create("Show save tree per level", false);

    config_->registerSetting(samplingFactorSetting_);
    config_->registerSetting(metallicSetting_);
    config_->registerSetting(roughnessSetting_);
    config_->registerSetting(worldRenderModeSetting_);
    config_->registerSetting(showSelectionMarkerSetting_);
    config_->registerSetting(showSaveTreePerLevelSetting_);

    dataView_ = std::make_shared<DataView>(config_);
    settingsWindow_ = std::make_unique<SettingsWindow>(config_);
    pakExplorer_ = std::make_shared<PakExplorer>(dataView_);
    aboutWindow_ = std::make_unique<AboutWindow>();

    // Fallback to HeightMap if no pak file is found.
    if (dataView_->pakManager() == nullptr) {
        worldRenderModeSetting_->setVal(WorldRenderMode::HeightMap);
    }

    fbo_ = std::make_unique<glowl::FramebufferObject>(10, 10, glowl::FramebufferObject::DEPTH32F);
    fbo_->createColorAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE); // color
    fbo_->createColorAttachment(GL_RGBA32F, GL_RGBA, GL_FLOAT);       // normals
    fbo_->createColorAttachment(GL_R32I, GL_RED_INTEGER, GL_INT);     // pick id
    fbo_->bind();
    if (!fbo_->checkStatus()) {
        throw std::runtime_error(fbo_->getLog());
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mainFbo_ = std::make_unique<glowl::FramebufferObject>(10, 10);
    mainFbo_->createColorAttachment(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
    mainFbo_->bind();
    if (!mainFbo_->checkStatus()) {
        throw std::runtime_error(mainFbo_->getLog());
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mainTexLayout_ = glowl::TextureLayout(GL_RGBA8, 10, 10, 1, GL_RGBA, GL_UNSIGNED_BYTE, 1,
        {
            {GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
            {GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
            {GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR},
            {GL_TEXTURE_MAG_FILTER, GL_LINEAR},
        },
        {});
    mainTex_ = std::make_unique<glowl::Texture2D>("mainTex", mainTexLayout_, nullptr, true);

    try {
        shaderQuad_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/quad.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/quad.frag")}});
    } catch (glowl::GLSLProgramException& e) {
        spdlog::error(e.what());
    }

    const std::vector<float> quadVertices{0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
    const std::vector<GLushort> quadIndices{0, 1, 2, 3};
    glowl::Mesh::VertexDataList<float> vertexInfoList{{quadVertices, {8, {{2, GL_FLOAT, GL_FALSE, 0}}}}};
    meshQuad_ = std::make_unique<glowl::Mesh>(vertexInfoList, quadIndices, GL_UNSIGNED_SHORT, GL_TRIANGLE_STRIP);

    worldRenderer_ = std::make_unique<WorldRenderer>(config_);
    mapTileRenderer_ = std::make_unique<MapTileRenderer>(config_, dataView_->pakManager());
    modelRenderer_ = std::make_unique<ModelRenderer>(config_, dataView_);

    selectionMarkerModel_ = std::make_unique<GltfModel>("models/ui/selection_marker.glb");
    try {
        selectionMarkerShader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/selectionmarker.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/selectionmarker.frag")}});
    } catch (glowl::GLSLProgramException& e) {
        spdlog::error(e.what());
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

Satisfactory3DMap::MapWindow::~MapWindow() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void Satisfactory3DMap::MapWindow::render() {
    if (!mapActive_) {
        resetInputStates();
    }

    renderTick();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderGui();
    renderFbo();
}

void Satisfactory3DMap::MapWindow::renderTick() {
    const double time = glfwGetTime();
    const double deltaT = time - lastTickTime_;
    lastTickTime_ = time;

    if (keyDownForward_) {
        camera_->keyPressedControl(AbstractCamera::KeyControl::Forward, deltaT);
    }
    if (keyDownBackward_) {
        camera_->keyPressedControl(AbstractCamera::KeyControl::Backward, deltaT);
    }
    if (keyDownLeft_) {
        camera_->keyPressedControl(AbstractCamera::KeyControl::Left, deltaT);
    }
    if (keyDownRight_) {
        camera_->keyPressedControl(AbstractCamera::KeyControl::Right, deltaT);
    }
    if (keyDownUp_) {
        camera_->keyPressedControl(AbstractCamera::KeyControl::Up, deltaT);
    }
    if (keyDownDown_) {
        camera_->keyPressedControl(AbstractCamera::KeyControl::Down, deltaT);
    }
    if (keyDownTurnLeft_) {
        camera_->keyPressedControl(AbstractCamera::KeyControl::TurnLeft, deltaT);
    }
    if (keyDownTurnRight_) {
        camera_->keyPressedControl(AbstractCamera::KeyControl::TurnRight, deltaT);
    }
    if (keyDownTurnUp_) {
        camera_->keyPressedControl(AbstractCamera::KeyControl::TurnUp, deltaT);
    }
    if (keyDownTurnDown_) {
        camera_->keyPressedControl(AbstractCamera::KeyControl::TurnDown, deltaT);
    }
}

void Satisfactory3DMap::MapWindow::renderGui() {
    ImGuiID dockspaceId = ImGui::DockSpaceOverViewport();

    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open")) {
            auto file = openFile("Select savegame file ...", ".", {"Satisfactory Save (*.sav)", "*.sav"});
            if (file.has_value()) {
                dataView_->openSave(file.value());
            }
        }
        if (ImGui::MenuItem("Save", nullptr, false, dataView_->hasSave())) {
            auto file = saveFile("Select savegame file ...", ".", {"Satisfactory Save (*.sav)", "*.sav"});
            if (file.has_value()) {
                dataView_->saveSave(file.value());
            }
        }
        ImGui::Separator();
        ImGui::MenuItem("Settings", nullptr, &settingsWindow_->show());
        ImGui::Separator();
        if (ImGui::MenuItem("Exit")) {
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View")) {
        ImGui::MenuItem("Save Header", nullptr, &showSaveHeader_);
        ImGui::Separator();
        ImGui::MenuItem("Camera Settings", nullptr, &showCameraWindow_);
        if (ImGui::MenuItem("Reset Camera")) {
            camera_->reset();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools")) {
        ImGui::MenuItem("Pak Explorer", nullptr, &pakExplorer_->show(), dataView_->pakManager() != nullptr);
        ImGui::Separator();
        if (ImGui::MenuItem("Export Save Text")) {
            auto file = saveFile("Select file ...", "savegame-export.txt");
            if (file.has_value()) {
                saveToTextFile(*dataView_->saveGame(), file.value());
            }
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("About")) {
            aboutWindow_->show();
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    static bool firstRun = true;
    if (firstRun) {
        firstRun = false;
        ImGui::DockBuilderRemoveNode(dockspaceId);
        ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);

        ImGuiID center = 0;
        // Left 20% | Center 55% | Right 25%
        ImGuiID dockIdLeft = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.2f, nullptr, &center);
        ImGuiID dockIdRight = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.3125f, nullptr, &center);
        ImGuiID dockIdCenterBottom = ImGui::DockBuilderSplitNode(center, ImGuiDir_Down, 0.25f, nullptr, &center);

        // Hide tab bar on map window
        auto* centerNode = ImGui::DockBuilderGetNode(center);
        centerNode->SetLocalFlags(centerNode->LocalFlags | ImGuiDockNodeFlags_HiddenTabBar);

        ImGui::DockBuilderDockWindow("3D Map", center);
        ImGui::DockBuilderDockWindow("Save Game", dockIdLeft);
        ImGui::DockBuilderDockWindow("SaveObject", dockIdRight);
        ImGui::DockBuilderDockWindow("Hex Editor", dockIdCenterBottom);
        ImGui::DockBuilderFinish(dockspaceId);
    }

    ImGui::Begin("Save Game");
    if (dataView_->hasSave()) {
        const auto& saveGame = dataView_->saveGame();
        if (!showSaveTreePerLevelSetting_->getVal()) {
            drawObjectTreeGui(dataView_->allRootNode());
        } else {
            if (ImGui::TreeNode("Level Main")) {
                drawObjectTreeGui(dataView_->persistentAndRuntimeRootNode());
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("LightweightBuildable")) {
                drawObjectTreeGui(dataView_->lightweightBuildableRootNode());
                ImGui::TreePop();
            }
            for (std::size_t i = 0; i < saveGame->mPerLevelDataMap.size(); i++) {
                if (ImGui::TreeNode(saveGame->mPerLevelDataMap.Keys[i].c_str())) {
                    drawObjectTreeGui(dataView_->levelRootNodes()[i]);
                    ImGui::TreePop();
                }
            }
        }
    } else {
        ImGui::Text("No Save Game loaded!");
    }
    ImGui::End();

    if (showSaveHeader_) {
        ImGui::SetNextWindowSize(ImVec2(500.0f, 400.0f), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Save Header", &showSaveHeader_);
        if (dataView_->hasSave()) {
            auto& header = dataView_->saveGame()->mSaveHeader;
            UI::EditorSectionWrap(0, "Save Header", [&]() {
                UI::EditorScalar("SaveHeaderVersion", ImGuiDataType_S32, &header.SaveHeaderVersion);
                UI::EditorScalar("SaveVersion", ImGuiDataType_S32, &header.SaveVersion);
                UI::EditorScalar("BuildVersion", ImGuiDataType_S32, &header.BuildVersion);
                UI::EditorString("SaveName", header.SaveName);
                UI::EditorString("MapName", header.MapName);
                UI::EditorString("MapOptions", header.MapOptions);
                UI::EditorString("SessionName", header.SessionName);
                UI::EditorScalar("PlayDurationSeconds", ImGuiDataType_S32, &header.PlayDurationSeconds);
                UI::EditorScalar("SaveDateTime", ImGuiDataType_S64, &header.SaveDateTime);
                UI::EditorScalar("SessionVisibility", ImGuiDataType_S8, &header.SessionVisibility);
                UI::EditorScalar("EditorObjectVersion", ImGuiDataType_S32, &header.EditorObjectVersion);
                UI::EditorString("ModMetadata", header.ModMetadata);
                UI::EditorBool("IsModdedSave", header.IsModdedSave);
                UI::EditorString("SaveIdentifier", header.SaveIdentifier);
                UI::EditorBool("IsPartitionedWorld", header.IsPartitionedWorld);
                UI::EditorShowText("SaveDataHash", header.SaveDataHash.toString().c_str());
                UI::EditorBool("IsCreativeModeEnabled", header.IsCreativeModeEnabled);
            });
        } else {
            ImGui::Text("No Save Game loaded!");
        }
        ImGui::End();
    }

    if (showCameraWindow_) {
        ImGui::SetNextWindowSize(ImVec2(300.0f, 150.0f), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Camera", &showCameraWindow_);
        camera_->renderGui();
        ImGui::End();
    }

    ImGui::Begin("SaveObject");
    if (dataView_->hasSelectedObject()) {
        UI::EventContext ctx;
        if (pakExplorer_->show()) {
            ctx.selectClass = std::bind_front(&PakExplorer::findAssetToClassName, pakExplorer_);
        }
        ctx.selectPath = std::bind_front(&DataView::selectPathName, dataView_);
        ctx.showBinData = [&](std::vector<char> data) {
            hexEditData_ = std::move(data);
            showHexEdit_ = true;
        };
        ctx.updateTransform = std::bind_front(&DataView::updateActor, dataView_);

        UI::ObjectEditor editor(ctx);
        editor.renderGui(dataView_->selectedObject());
    } else {
        ImGui::Text("No object selected!");
    }
    ImGui::End();

    if (showHexEdit_ && !hexEditData_.empty()) {
        ImGui::Begin("Hex Editor", &showHexEdit_);
        static MemoryEditor hexEditor;
        hexEditor.OptShowDataPreview = true;
        hexEditor.DrawContents(hexEditData_.data(), hexEditData_.size());
        ImGui::End();
    }

    settingsWindow_->renderGui();
    pakExplorer_->renderGui();
    aboutWindow_->renderGui();

    // Add 3D map window last that it becomes the initially active window.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
    ImGui::Begin("3D Map");
    mapViewLeft_ = static_cast<int>(ImGui::GetCursorScreenPos().x);
    mapViewTop_ = static_cast<int>(ImGui::GetCursorScreenPos().y);
    const ImVec2 cursorPos = ImGui::GetCursorPos();
    const ImVec2 size = ImGui::GetContentRegionAvail();
    mapViewWidth_ = static_cast<int>(size.x);
    mapViewHeight_ = static_cast<int>(size.y);
    // TODO The texture reference we store here, will probably be deleted on resize events.
    ImTextureID tex = static_cast<ImTextureID>(mainTex_->getName());
    ImGui::Image(tex, size, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::SetCursorPos(cursorPos);
    ImGui::Text("%.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    ImGui::SetCursorPos(cursorPos);
    ImGui::InvisibleButton("3D Map Button", ImGui::GetContentRegionAvail(), ImGuiButtonFlags_None);
    mapActive_ = ImGui::IsItemHovered() || ImGui::IsItemActive();
    if (mapActive_) {
        ImGui::SetNextFrameWantCaptureKeyboard(false);
        ImGui::SetNextFrameWantCaptureMouse(false);
    }
    ImGui::End();
    ImGui::PopStyleVar(2);

    // Alerts
    if (!dataView_->showErrors().empty()) {
        ImGui::OpenPopup("Error");
    }
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("%s", dataView_->showErrors().front().c_str());
        ImGui::Separator();
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            dataView_->showErrors().pop_front();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }
}

void Satisfactory3DMap::MapWindow::renderFbo() {

    const auto& samplingFactor = samplingFactorSetting_->getVal();
    int cleanWidth = std::max(1, static_cast<int>(static_cast<float>(mapViewWidth_) * samplingFactor));
    int cleanHeight = std::max(1, static_cast<int>(static_cast<float>(mapViewHeight_) * samplingFactor));

    if (cleanWidth != mainFbo_->getWidth() || cleanHeight != mainFbo_->getHeight()) {
        fbo_->resize(cleanWidth, cleanHeight);
        mainFbo_->resize(cleanWidth, cleanHeight);

        mainTexLayout_.width = cleanWidth;
        mainTexLayout_.height = cleanHeight;
        mainTex_->reload(mainTexLayout_, nullptr, true);

        float aspect = static_cast<float>(cleanWidth) / static_cast<float>(cleanHeight);
        projMx_ = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 10000.0f);
    }

    glViewport(0, 0, cleanWidth, cleanHeight);

    fbo_->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    GLubyte clearColor0[4]{0, 0, 0, 0}; // Clear to full transparency.
    GLfloat clearColor1[4]{0.0f, 0.0f, 0.0f, 0.0f};
    GLint clearColor2[1]{-1};
    glClearTexImage(fbo_->getColorAttachment(0)->getName(), 0, GL_RGBA, GL_UNSIGNED_BYTE, clearColor0);
    glClearTexImage(fbo_->getColorAttachment(1)->getName(), 0, GL_RGBA, GL_FLOAT, clearColor1);
    glClearTexImage(fbo_->getColorAttachment(2)->getName(), 0, GL_RED_INTEGER, GL_INT, clearColor2);

    if (worldRenderModeSetting_->getVal() == WorldRenderMode::HeightMap) {
        worldRenderer_->render(projMx_, camera_->viewMx());
    } else if (worldRenderModeSetting_->getVal() == WorldRenderMode::TileMap) {
        mapTileRenderer_->render(projMx_, camera_->viewMx());
    }

    if (dataView_->hasSave()) {
        modelRenderer_->render(projMx_, camera_->viewMx(), dataView_->selectedObjectId());

        if (showSelectionMarkerSetting_->getVal() && dataView_->hasSelectedObject()) {
            if (dataView_->selectedObject()->isActor()) {
                selectionMarkerShader_->use();
                selectionMarkerShader_->setUniform("projMx", projMx_);
                selectionMarkerShader_->setUniform("viewMx", camera_->viewMx());
                selectionMarkerShader_->setUniform("actor_pos",
                    dataView_->selectedObject()->getTranslation() * glm::vec3(0.01f));
                selectionMarkerModel_->draw();
                glUseProgram(0);
            }
        }
    }

    mainFbo_->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    GLfloat clearColor[4]{0.2f, 0.2f, 0.2f, 1.0f}; // Background color
    glClearTexImage(mainFbo_->getColorAttachment(0)->getName(), 0, GL_RGBA, GL_FLOAT, clearColor);

    shaderQuad_->use();
    shaderQuad_->setUniform("projMxQuad", glm::ortho(0.0f, 1.0f, 0.0f, 1.0f));
    shaderQuad_->setUniform("invProjMx", glm::inverse(projMx_));
    shaderQuad_->setUniform("invViewMx", glm::inverse(camera_->viewMx()));
    shaderQuad_->setUniform("metallic", metallicSetting_->getVal());
    shaderQuad_->setUniform("roughness", roughnessSetting_->getVal());

    glActiveTexture(GL_TEXTURE0);
    fbo_->bindColorbuffer(0);
    shaderQuad_->setUniform("texAlbedo", 0);

    glActiveTexture(GL_TEXTURE1);
    fbo_->bindColorbuffer(1);
    shaderQuad_->setUniform("texNormal", 1);

    glActiveTexture(GL_TEXTURE2);
    fbo_->bindDepthbuffer();
    shaderQuad_->setUniform("texDepth", 2);

    meshQuad_->draw();

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, framebufferWidth_, framebufferHeight_);

    // TODO copy the result to an extra texture, to use mip maps for supersampling.
    // Once the glowl framebuffer interface supports using mipmaps directly we can remove this again.
    mainFbo_->bindToRead(0);
    glCopyTextureSubImage2D(mainTex_->getName(), 0, 0, 0, 0, 0, mainTexLayout_.width, mainTexLayout_.height);
    mainTex_->updateMipmaps();
}

void Satisfactory3DMap::MapWindow::keyEvent(int key, [[maybe_unused]] int scancode, int action,
    [[maybe_unused]] int mods) {
    switch (key) {
        case GLFW_KEY_W:
            keyDownForward_ = action == GLFW_PRESS || action == GLFW_REPEAT;
            break;
        case GLFW_KEY_S:
            keyDownBackward_ = action == GLFW_PRESS || action == GLFW_REPEAT;
            break;
        case GLFW_KEY_A:
            keyDownLeft_ = action == GLFW_PRESS || action == GLFW_REPEAT;
            break;
        case GLFW_KEY_D:
            keyDownRight_ = action == GLFW_PRESS || action == GLFW_REPEAT;
            break;
        case GLFW_KEY_SPACE:
            keyDownUp_ = action == GLFW_PRESS || action == GLFW_REPEAT;
            break;
        case GLFW_KEY_C:
            keyDownDown_ = action == GLFW_PRESS || action == GLFW_REPEAT;
            break;
        case GLFW_KEY_LEFT:
            keyDownTurnLeft_ = action == GLFW_PRESS || action == GLFW_REPEAT;
            break;
        case GLFW_KEY_RIGHT:
            keyDownTurnRight_ = action == GLFW_PRESS || action == GLFW_REPEAT;
            break;
        case GLFW_KEY_UP:
            keyDownTurnUp_ = action == GLFW_PRESS || action == GLFW_REPEAT;
            break;
        case GLFW_KEY_DOWN:
            keyDownTurnDown_ = action == GLFW_PRESS || action == GLFW_REPEAT;
            break;
        default:
            break;
    }

    // Do not count mouse click as selection if any button was pressed.
    mouseMoved_ = true;
}

void Satisfactory3DMap::MapWindow::mouseButtonEvent(int button, int action, int mods) {
    // Selection control
    if (button == GLFW_MOUSE_BUTTON_LEFT && mods == 0) {
        if (action == GLFW_PRESS) {
            mouseMoved_ = false;
        } else if (action == GLFW_RELEASE && !mouseMoved_) {
            const auto& samplingFactor = samplingFactorSetting_->getVal();
            float xpos = static_cast<float>(static_cast<int>(mouseX_) - mapViewLeft_) * samplingFactor;
            float ypos =
                static_cast<float>(mapViewHeight_ - (static_cast<int>(mouseY_) - mapViewTop_)) * samplingFactor;
            GLint x = std::clamp(static_cast<int>(xpos), 0, fbo_->getWidth() - 1);
            GLint y = std::clamp(static_cast<int>(ypos), 0, fbo_->getHeight() - 1);
            fbo_->bindToRead(2);
            int id;
            glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, reinterpret_cast<void*>(&id));
            glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
            dataView_->selectObject(id);
        }
    }

    // Camera control
    if (action == GLFW_RELEASE) {
        cameraControlMode_ = AbstractCamera::MouseControlMode::None;
        enableMouseCursor();
    }
    if (action == GLFW_PRESS && mods == 0) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            cameraControlMode_ = AbstractCamera::MouseControlMode::Left;
        } else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            cameraControlMode_ = AbstractCamera::MouseControlMode::Middle;
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            cameraControlMode_ = AbstractCamera::MouseControlMode::Right;
        }
    }
}

void Satisfactory3DMap::MapWindow::mouseMoveEvent(double xpos, double ypos) {
    mouseMoved_ = true;

    // Camera control
    if (cameraControlMode_ != AbstractCamera::MouseControlMode::None) {
        camera_->mouseMoveControl(cameraControlMode_,
            {mouseX_ - static_cast<double>(mapViewLeft_), mouseY_ - static_cast<double>(mapViewTop_)},
            {xpos - static_cast<double>(mapViewLeft_), ypos - static_cast<double>(mapViewTop_)},
            {mapViewWidth_, mapViewHeight_});
    }

    // Disable mouse cursor, while left button is pressed and moved for FPS like camera control.
    if (cameraControlMode_ == AbstractCamera::MouseControlMode::Left) {
        disableMouseCursor();
    } else {
        enableMouseCursor();
    }

    mouseX_ = xpos;
    mouseY_ = ypos;
}

void Satisfactory3DMap::MapWindow::mouseScrollEvent(double xoffset, double yoffset) {
    camera_->mouseScrollControl({xoffset, yoffset});
}

void Satisfactory3DMap::MapWindow::dropEvent(const std::vector<std::string>& paths) {
    if (paths.size() != 1) {
        spdlog::error("Can only read a single file!");
    }
    dataView_->openSave(paths[0]);
}

void Satisfactory3DMap::MapWindow::drawObjectTreeGui(const DataView::SaveNode& n) {
    for (const auto& child : n.childNodes) {
        std::string counts =
            " (A:" + std::to_string(child.second.numActors) + " O:" + std::to_string(child.second.numObjects) + ")";
        if (UI::TreeNodeSmall((child.first + counts).c_str())) {
            drawObjectTreeGui(child.second);
            ImGui::TreePop();
        }
    }
    for (const auto& obj : n.objects) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        if (obj == dataView_->selectedObject()) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        const auto id = reinterpret_cast<void*>(obj.get());
        UI::TreeNodeSmallEx(id, flags, "[%s] %s", obj->isActor() ? "A" : "0", obj->pathName().c_str());
        if (ImGui::IsItemClicked()) {
            dataView_->selectObject(obj);
        }
    }
}

void Satisfactory3DMap::MapWindow::enableMouseCursor() {
    if (mouseHidden_) {
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        if (glfwRawMouseMotionSupported()) {
            glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
        }
        mouseHidden_ = false;
    }
}

void Satisfactory3DMap::MapWindow::disableMouseCursor() {
    if (!mouseHidden_) {
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        if (glfwRawMouseMotionSupported()) {
            glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseHidden_ = true;
    }
}

void Satisfactory3DMap::MapWindow::resetInputStates() {
    cameraControlMode_ = AbstractCamera::MouseControlMode::None;
    keyDownForward_ = false;
    keyDownBackward_ = false;
    keyDownLeft_ = false;
    keyDownRight_ = false;
    keyDownUp_ = false;
    keyDownDown_ = false;
    keyDownTurnLeft_ = false;
    keyDownTurnRight_ = false;
    keyDownTurnUp_ = false;
    keyDownTurnDown_ = false;
    enableMouseCursor();
}
