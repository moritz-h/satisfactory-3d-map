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
        std::unique_ptr<glowl::GLSLProgram> shader_;

        std::unique_ptr<Model> modelCube_;
        std::unique_ptr<Model> modelFoundation8x4_;
        std::unique_ptr<Model> modelFoundation8x2_;
        std::unique_ptr<Model> modelFoundation8x1_;

        std::unique_ptr<glowl::BufferObject> posBufferCube_;
        std::unique_ptr<glowl::BufferObject> posBufferFoundation8x4_;
        std::unique_ptr<glowl::BufferObject> posBufferFoundation8x2_;
        std::unique_ptr<glowl::BufferObject> posBufferFoundation8x1_;
        int numActorsCube_;
        int numActorsFoundation8x4_;
        int numActorsFoundation8x2_;
        int numActorsFoundation8x1_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MODELRENDERER_H
