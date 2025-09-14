#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "SatisfactorySave/Pak/PakManager.h"

#include "../Config/BoolSetting.h"
#include "../Config/Configuration.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap {

    class WorldRenderer {
    public:
        explicit WorldRenderer(const std::shared_ptr<Configuration>& config,
            const std::shared_ptr<s::PakManager>& pakManager);
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
        GLuint pakTexHeight_ = 0;
        GLuint pakTexMap00_ = 0;
        GLuint pakTexMap01_ = 0;
        GLuint pakTexMap10_ = 0;
        GLuint pakTexMap11_ = 0;

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
