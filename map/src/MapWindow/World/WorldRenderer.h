#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "../Config/BoolSetting.h"
#include "../Config/Configuration.h"

namespace Satisfactory3DMap {

    class WorldRenderer {
    public:
        explicit WorldRenderer(const std::shared_ptr<Configuration>& config);
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

        std::shared_ptr<BoolSetting> useWorldTexSetting_;
        std::shared_ptr<BoolSetting> wireframeSetting_;
    };
} // namespace Satisfactory3DMap
