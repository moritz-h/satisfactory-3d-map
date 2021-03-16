#ifndef SATISFACTORY3DMAP_MODEL_H
#define SATISFACTORY3DMAP_MODEL_H

#include <memory>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

namespace Satisfactory3DMap {

    class Model {
    public:
        explicit Model(const std::string& resourceName);
        ~Model() = default;

        void draw(std::size_t instanceCount = 1) {
            if (mesh_ != nullptr) {
                mesh_->draw(instanceCount);
            }
        }

        void bindTexture() {
            if (tex_ != nullptr) {
                tex_->bindTexture();
            }
        }

        [[nodiscard]] const glm::mat4& modelMx() const {
            return modelMx_;
        }

    protected:
        std::unique_ptr<glowl::Mesh> mesh_;
        std::unique_ptr<glowl::Texture2D> tex_;
        glm::mat4 modelMx_;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MODEL_H
