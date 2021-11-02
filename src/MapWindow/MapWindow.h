#ifndef SATISFACTORY3DMAP_MAPWINDOW_H
#define SATISFACTORY3DMAP_MAPWINDOW_H

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <glowl/glowl.h>

#include "BaseWindow.h"
#include "Camera/AbstractCamera.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "PropertyTableGuiRenderer.h"
#include "SaveGame/SaveGame.h"
#include "WorldRenderer.h"

namespace Satisfactory3DMap {

    class MapWindow final : public BaseWindow {
    public:
        MapWindow();
        ~MapWindow();

        void openSave(const std::filesystem::path& file);
        void saveSave(const std::filesystem::path& file);

    protected:
        void render() override;
        void renderTick();
        void renderGui();
        void renderFbo();

        void keyEvent(int key, int scancode, int action, int mods) override;
        void mouseButtonEvent(int button, int action, int mods) override;
        void mouseMoveEvent(double xpos, double ypos) override;
        void mouseScrollEvent(double xoffset, double yoffset) override;
        void dropEvent(const std::vector<std::string>& paths) override;

        void drawObjectTreeGui(const Satisfactory3DMap::SaveGame::SaveNode& n);

        void showMouse();
        void hideMouse();

        void resetInputStates();

        std::unique_ptr<SaveGame> savegame_;

        std::unique_ptr<glowl::FramebufferObject> mainFbo_;
        std::unique_ptr<glowl::FramebufferObject> fbo_;
        std::unique_ptr<glowl::GLSLProgram> shaderQuad_;
        std::unique_ptr<glowl::Mesh> meshQuad_;

        std::unique_ptr<WorldRenderer> worldRenderer_;
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
        double mouseX_;
        double mouseY_;
        bool mouseHidden_;
        bool mouseMoved_;

        AbstractCamera::MouseControlMode cameraControlMode_;
        std::unique_ptr<AbstractCamera> camera_;
        glm::mat4 projMx_;

        int selectedObject_;

        float metallic_;
        float roughness_;

        std::unique_ptr<Model> selectionMarkerModel_;
        std::unique_ptr<glowl::GLSLProgram> selectionMarkerShader_;
        bool showSelectionMarker_;

        bool showHexEdit_;
        std::vector<char> hexEditData_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPWINDOW_H
