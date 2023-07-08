#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "SatisfactorySave/SaveGame.h"

#include "AboutWindow.h"
#include "BaseWindow.h"
#include "Camera/AbstractCamera.h"
#include "Config/BoolSetting.h"
#include "Config/EnumSetting.h"
#include "Config/FloatSetting.h"
#include "Config/SettingsWindow.h"
#include "DataView/DataView.h"
#include "ModelRenderer.h"
#include "OpenGL/GltfModel.h"
#include "PakExplorer.h"
#include "UI/PropertyTableGuiRenderer.h"
#include "World/MapTileRenderer.h"
#include "World/WorldRenderer.h"

namespace Satisfactory3DMap {

    class MapWindow final : public BaseWindow {
    public:
        MapWindow();
        ~MapWindow();

        [[nodiscard]] const std::shared_ptr<DataView>& dataView() const {
            return dataView_;
        }

    protected:
        enum WorldRenderMode : int {
            None = 0,
            HeightMap = 1,
            TileMap = 2,
        };

        void render() override;
        void renderTick();
        void renderGui();
        void renderFbo();

        void keyEvent(int key, int scancode, int action, int mods) override;
        void mouseButtonEvent(int button, int action, int mods) override;
        void mouseMoveEvent(double xpos, double ypos) override;
        void mouseScrollEvent(double xoffset, double yoffset) override;
        void dropEvent(const std::vector<std::string>& paths) override;

        void drawObjectTreeGui(const SatisfactorySave::SaveGame::SaveNode& n);

        void enableMouseCursor();
        void disableMouseCursor();

        void resetInputStates();

        std::shared_ptr<DataView> dataView_;
        std::unique_ptr<SettingsWindow> settingsWindow_;
        std::unique_ptr<PakExplorer> pakExplorer_;
        std::unique_ptr<AboutWindow> aboutWindow_;

        std::unique_ptr<glowl::FramebufferObject> mainFbo_;
        glowl::TextureLayout mainTexLayout_;
        std::unique_ptr<glowl::Texture2D> mainTex_;
        std::unique_ptr<glowl::FramebufferObject> fbo_;
        std::unique_ptr<glowl::GLSLProgram> shaderQuad_;
        std::unique_ptr<glowl::Mesh> meshQuad_;

        std::unique_ptr<WorldRenderer> worldRenderer_;
        std::unique_ptr<MapTileRenderer> mapTileRenderer_;
        std::unique_ptr<ModelRenderer> modelRenderer_;

        std::unique_ptr<PropertyTableGuiRenderer> propertyTableGuiRenderer_;

        int mapViewLeft_;
        int mapViewTop_;
        int mapViewWidth_;
        int mapViewHeight_;

        double lastTickTime_;

        bool mapActive_;
        bool keyDownForward_;
        bool keyDownBackward_;
        bool keyDownLeft_;
        bool keyDownRight_;
        bool keyDownUp_;
        bool keyDownDown_;
        bool keyDownTurnLeft_;
        bool keyDownTurnRight_;
        bool keyDownTurnUp_;
        bool keyDownTurnDown_;
        double mouseX_;
        double mouseY_;
        bool mouseHidden_;
        bool mouseMoved_;

        AbstractCamera::MouseControlMode cameraControlMode_;
        std::unique_ptr<AbstractCamera> camera_;
        glm::mat4 projMx_;

        std::shared_ptr<EnumSetting<float>> samplingFactorSetting_;
        std::shared_ptr<FloatSetting> metallicSetting_;
        std::shared_ptr<FloatSetting> roughnessSetting_;
        std::shared_ptr<EnumSetting<WorldRenderMode>> worldRenderModeSetting_;
        std::shared_ptr<BoolSetting> showSelectionMarkerSetting_;
        std::shared_ptr<BoolSetting> showSaveTreePerLevelSetting_;

        std::unique_ptr<GltfModel> selectionMarkerModel_;
        std::unique_ptr<glowl::GLSLProgram> selectionMarkerShader_;

        bool showHexEdit_;
        std::vector<char> hexEditData_;
    };
} // namespace Satisfactory3DMap
