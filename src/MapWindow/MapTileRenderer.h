#ifndef SATISFACTORY3DMAP_MAPTILERENDERER_H
#define SATISFACTORY3DMAP_MAPTILERENDERER_H

#include <memory>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "Model.h"

namespace Satisfactory3DMap {

    class MapTileRenderer {
    public:
        MapTileRenderer();
        ~MapTileRenderer() = default;

        void render(const glm::mat4& projMx, const glm::mat4& viewMx);

        bool& wireframe() {
            return wireframe_;
        };

        bool& show() {
            return show_;
        }

    protected:
        struct MapTileInfo {
            std::string filename;
            int x;
            int y;
            bool offset;
        };

        struct MapTileData {
            GLuint vao;
            int indices;
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
        bool show_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPTILERENDERER_H
