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

        bool& wireframeGltf() {
            return wireframeGltf_;
        };

        bool& wireframeMesh() {
            return wireframeMesh_;
        };

        bool& showGltf() {
            return showGltf_;
        }

        bool& showMesh() {
            return showMesh_;
        }

    protected:
        struct MapTileInfo {
            std::string filename;
            int x;
            int y;
            bool offset;
        };

        struct GltfMapTileData {
            std::shared_ptr<Model> model; // TODO make unique
            float x;
            float y;
            bool offset;
            int tileX;
            int tileY;
        };

        struct MapTileData {
            GLuint vao;
            int indices;
            GLuint tex;
            float x;
            float y;
            bool offset;
            int tileX;
            int tileY;
        };

        void renderGltf(const glm::mat4& projMx, const glm::mat4& viewMx);
        void renderMesh(const glm::mat4& projMx, const glm::mat4& viewMx);

        std::unique_ptr<glowl::GLSLProgram> shaderGltf_;
        std::vector<GltfMapTileData> mapTilesGltf_;

        std::unique_ptr<glowl::GLSLProgram> shaderMesh_;
        std::vector<MapTileData> mapTiles_;

        bool wireframeGltf_;
        bool wireframeMesh_;
        bool showGltf_;
        bool showMesh_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPTILERENDERER_H
