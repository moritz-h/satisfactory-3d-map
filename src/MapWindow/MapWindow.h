#ifndef SATISFACTORY3DMAP_MAPWINDOW_H
#define SATISFACTORY3DMAP_MAPWINDOW_H

#include <memory>

#include <glowl/glowl.h>

#include "BaseWindow.h"
#include "Camera/Camera.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "SaveGame/SaveGame.h"
#include "WorldRenderer.h"

namespace Satisfactory3DMap {

    class MapWindow : public BaseWindow {
    public:
        MapWindow();
        ~MapWindow();

        void openSave(const std::string& filename);

    protected:
        void render() override;
        void renderGui();
        void renderFbo();

        void resizeEvent(int width, int height) override;
        void mouseButtonEvent(int button, int action, int mods) override;
        void mouseMoveEvent(double xpos, double ypos) override;
        void mouseScrollEvent(double xoffset, double yoffset) override;
        void dropEvent(const std::vector<std::string>& paths) override;

        void drawObjectTreeGui(const Satisfactory3DMap::SaveGame::SaveNode& n);
        void drawPropertyValueGui(const Property& p);

        std::unique_ptr<SaveGame> savegame_;

        std::unique_ptr<glowl::FramebufferObject> fbo_;
        std::unique_ptr<glowl::GLSLProgram> shaderQuad_;
        std::unique_ptr<glowl::Mesh> meshQuad_;

        std::unique_ptr<WorldRenderer> worldRenderer_;
        std::unique_ptr<ModelRenderer> modelRenderer_;

        double mouseX_;
        double mouseY_;

        Camera::MouseControlMode cameraControlMode_;
        Camera camera_;
        glm::mat4 projMx_;

        int selectedObject_;

        float metallic_;
        float roughness_;

        bool showHexEdit_;
        std::vector<char> hexEditData_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPWINDOW_H
