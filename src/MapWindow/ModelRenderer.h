#ifndef SATISFACTORY3DMAP_MODELRENDERER_H
#define SATISFACTORY3DMAP_MODELRENDERER_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "Model.h"
#include "SaveGame/SaveGame.h"

namespace Satisfactory3DMap {

    class ModelRenderer {
    public:
        ModelRenderer();
        ~ModelRenderer() = default;

        void loadSave(const SaveGame& saveGame);

        void render(const glm::mat4& projMx, const glm::mat4& viewMx);

        bool& wireframe() {
            return wireframe_;
        };

    protected:
        struct ModelPath {
            ModelPath(const std::string& modelFile, std::vector<std::string> savePaths)
                : model(std::make_unique<Model>(modelFile)),
                  savePaths(std::move(savePaths)) {}
            std::unique_ptr<Model> model;
            std::vector<std::string> savePaths;
        };
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

        std::unique_ptr<glowl::GLSLProgram> shader_;
        std::unique_ptr<glowl::GLSLProgram> splineShader_;

        std::vector<ModelPath> models_;
        std::vector<ModelData> modelDataList_;

        std::vector<ModelPath> splineModels_;
        std::vector<SplineModelData> splineModelDataList_;

        bool wireframe_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MODELRENDERER_H
