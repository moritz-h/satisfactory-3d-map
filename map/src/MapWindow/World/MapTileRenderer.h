#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "SatisfactorySave/Pak/PakManager.h"

#include "../Config/BoolSetting.h"
#include "../Config/Configuration.h"
#include "../OpenGL/Texture.h"

namespace Satisfactory3DMap {

    class MapTileRenderer {
    public:
        explicit MapTileRenderer(const std::shared_ptr<Configuration>& config,
            const std::shared_ptr<SatisfactorySave::PakManager>& pakManager);
        ~MapTileRenderer() = default;

        void render(const glm::mat4& projMx, const glm::mat4& viewMx);

    protected:
        struct MapTileData {
            std::shared_ptr<glowl::Mesh> mesh;
            std::unique_ptr<OGLTexture2D> texBaseColor;
            std::unique_ptr<OGLTexture2D> texNormal;
            glm::mat4 modelMx;
            glm::mat3 normalMx;
        };

        std::unique_ptr<glowl::GLSLProgram> meshShader_;
        std::unique_ptr<glowl::GLSLProgram> flatShader_;
        std::vector<MapTileData> mapTiles_;

        std::shared_ptr<BoolSetting> wireframeSetting_;
        std::shared_ptr<BoolSetting> faceNormalsSetting_;
    };
} // namespace Satisfactory3DMap
