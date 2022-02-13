#ifndef SATISFACTORY3DMAP_MODELRENDERER_H
#define SATISFACTORY3DMAP_MODELRENDERER_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "ModelManager.h"
#include "SaveGame/SaveGame.h"

namespace Satisfactory3DMap {

    class ModelRenderer {
    public:
        ModelRenderer();
        ~ModelRenderer() = default;

        void loadSave(const SaveGame& saveGame);

        void updateActor(const SaveActor& actor);

        void render(const glm::mat4& projMx, const glm::mat4& viewMx, int selectedId);

        bool& wireframe() {
            return wireframe_;
        };

    protected:
        struct ModelData {
            ModelData() : numActors(0){};
            std::unique_ptr<glowl::BufferObject> idBuffer;
            std::unique_ptr<glowl::BufferObject> transformBuffer;
            int numActors;
        };
        struct SplineModelData {
            SplineModelData() : numInstances(0){};
            std::unique_ptr<glowl::BufferObject> splineSegments;
            std::unique_ptr<glowl::BufferObject> instanceData;
            int numInstances;
        };

        std::unique_ptr<ModelManager> manager_;

        std::unique_ptr<glowl::GLSLProgram> shader_;
        std::unique_ptr<glowl::GLSLProgram> splineShader_;

        std::vector<ModelData> modelDataList_;
        std::vector<SplineModelData> splineModelDataList_;

        std::unordered_map<std::size_t, std::tuple<std::size_t, std::size_t>> actorBufferPositions_;

        bool wireframe_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MODELRENDERER_H
