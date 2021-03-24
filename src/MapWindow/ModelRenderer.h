#ifndef SATISFACTORY3DMAP_MODELRENDERER_H
#define SATISFACTORY3DMAP_MODELRENDERER_H

#include <memory>

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

    protected:
        struct ModelPath {
            ModelPath(const std::string& modelFile, std::string savePath)
                : model(std::make_unique<Model>(modelFile)), savePath(std::move(savePath)) {}
            std::unique_ptr<Model> model;
            std::string savePath;
        };
        struct ModelData {
            ModelData() : numActors(0){};
            std::unique_ptr<glowl::BufferObject> posBuffer;
            int numActors;
        };

        std::unique_ptr<glowl::GLSLProgram> shader_;

        std::vector<ModelPath> models_;
        std::vector<ModelData> modelDataList_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MODELRENDERER_H
