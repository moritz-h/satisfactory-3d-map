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

        bool& show() {
            return show_;
        }

    protected:
        struct MapTileData {
            std::shared_ptr<Model> model; // TODO make unique
            float x;
            float y;
            bool offset;
        };

        std::unique_ptr<glowl::GLSLProgram> shader_;
        std::vector<MapTileData> mapTiles_;

        bool show_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAPTILERENDERER_H
