#ifndef SATISFACTORY3DMAP_MAPWINDOW_H
#define SATISFACTORY3DMAP_MAPWINDOW_H

#include <memory>

#include <glowl/glowl.h>

#include "BaseWindow.h"

namespace Satisfactory3DMap {

    class MapWindow : public BaseWindow {
    public:
        MapWindow();
        ~MapWindow();

    protected:
        void render() override;

        std::unique_ptr<glowl::GLSLProgram> shaderBox_;
        std::unique_ptr<glowl::Mesh> meshBox_;
        std::unique_ptr<glowl::Texture2D> texBox_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPWINDOW_H
