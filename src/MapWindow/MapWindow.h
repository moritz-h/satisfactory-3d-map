#ifndef SATISFACTORY3DMAP_MAPWINDOW_H
#define SATISFACTORY3DMAP_MAPWINDOW_H

#include <memory>

#include <glowl/glowl.h>

#include "BaseWindow.h"
#include "Camera/Camera.h"
#include "Model.h"
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

        void mouseButtonEvent(int button, int action, int mods) override;
        void mouseMoveEvent(double xpos, double ypos) override;
        void mouseScrollEvent(double xoffset, double yoffset) override;
        void dropEvent(const std::vector<std::string>& paths) override;

        std::unique_ptr<SaveGame> savegame_;

        std::unique_ptr<glowl::FramebufferObject> fbo_;

        std::unique_ptr<glowl::GLSLProgram> shaderQuad_;
        std::unique_ptr<glowl::GLSLProgram> shaderModels_;

        std::unique_ptr<glowl::Mesh> meshQuad_;

        std::unique_ptr<WorldRenderer> worldRenderer_;

        std::unique_ptr<Model> modelCube_;
        std::unique_ptr<Model> modelFoundation8x4_;
        std::unique_ptr<Model> modelFoundation8x2_;
        std::unique_ptr<Model> modelFoundation8x1_;

        double mouseX_;
        double mouseY_;

        Camera::MouseControlMode cameraControlMode_;
        Camera camera_;

        std::unique_ptr<glowl::BufferObject> posBufferCube_;
        std::unique_ptr<glowl::BufferObject> posBufferFoundation8x4_;
        std::unique_ptr<glowl::BufferObject> posBufferFoundation8x2_;
        std::unique_ptr<glowl::BufferObject> posBufferFoundation8x1_;
        int numActorsCube_;
        int numActorsFoundation8x4_;
        int numActorsFoundation8x2_;
        int numActorsFoundation8x1_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPWINDOW_H
