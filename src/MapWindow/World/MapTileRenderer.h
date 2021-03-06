#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "../Config/BoolSetting.h"
#include "../Config/Configuration.h"
#include "../OpenGL/StaticMeshVAO.h"
#include "IO/Pak/PakManager.h"

namespace Satisfactory3DMap {

    class MapTileRenderer {
    public:
        explicit MapTileRenderer(const std::shared_ptr<Configuration>& config,
            const std::shared_ptr<PakManager>& pakManager);
        ~MapTileRenderer() = default;

        void render(const glm::mat4& projMx, const glm::mat4& viewMx);

    protected:
        struct MapTileInfo {
            std::string filename;
            int x;
            int y;
            bool offset;
        };

        struct MapTileData {
            std::unique_ptr<StaticMeshVAO> mesh;
            GLuint texD;
            GLuint texN;
            float x;
            float y;
            bool offset;
            int tileX;
            int tileY;
        };

        std::unique_ptr<glowl::GLSLProgram> shader_;
        std::vector<MapTileData> mapTiles_;

        std::shared_ptr<BoolSetting> wireframeSetting_;
    };
} // namespace Satisfactory3DMap
