#ifndef SATISFACTORY3DMAP_MAPWINDOW_H
#define SATISFACTORY3DMAP_MAPWINDOW_H

#include <memory>

#include <glowl/glowl.h>

#include "BaseWindow.h"
#include "Camera/Camera.h"
#include "Model.h"
#include "SaveGame/SaveGame.h"

namespace Satisfactory3DMap {

    class MapWindow : public BaseWindow {
    public:
        MapWindow();
        ~MapWindow();

        void openSave(const std::string& filename);

    protected:
        void render() override;

        void mouseButtonEvent(int button, int action, int mods) override;
        void mouseMoveEvent(double xpos, double ypos) override;
        void mouseScrollEvent(double xoffset, double yoffset) override;
        void dropEvent(const std::vector<std::string>& paths) override;

        std::unique_ptr<SaveGame> savegame_;

        std::unique_ptr<glowl::GLSLProgram> shaderBox_;
        std::unique_ptr<Model> modelBox_;

        double mouseX_;
        double mouseY_;

        Camera::MouseControlMode cameraControlMode_;
        Camera camera_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPWINDOW_H
