#ifndef SATISFACTORY3DMAP_WORLDRENDERER_H
#define SATISFACTORY3DMAP_WORLDRENDERER_H

#include <memory>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

namespace Satisfactory3DMap {

    class WorldRenderer {
    public:
        WorldRenderer();
        ~WorldRenderer() = default;

        void render(const glm::mat4& projMx, const glm::mat4& viewMx);

    protected:
        std::unique_ptr<glowl::GLSLProgram> shader_;
        GLuint vaEmpty_;

        std::unique_ptr<glowl::Texture2D> texHeight_;
        std::unique_ptr<glowl::Texture2D> texMap00_;
        std::unique_ptr<glowl::Texture2D> texMap01_;
        std::unique_ptr<glowl::Texture2D> texMap10_;
        std::unique_ptr<glowl::Texture2D> texMap11_;

        int texHeightWidth_;
        int texHeightHeight_;

        int tessLevelInner_;
        int tessLevelOuter_;
        int numInstancesX_;
        int numInstancesY_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_WORLDRENDERER_H
