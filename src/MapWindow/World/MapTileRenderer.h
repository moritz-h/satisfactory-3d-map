#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "../DataView/PakManager.h"
#include "../OpenGL/StaticMeshVAO.h"

namespace Satisfactory3DMap {

    class MapTileRenderer {
    public:
        explicit MapTileRenderer(const std::shared_ptr<PakManager>& pakManager);
        ~MapTileRenderer() = default;

        void render(const glm::mat4& projMx, const glm::mat4& viewMx);

        bool& wireframe() {
            return wireframe_;
        };

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

        bool wireframe_;
    };
} // namespace Satisfactory3DMap
