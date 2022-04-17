#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

namespace Satisfactory3DMap {

    class GltfModel {
    public:
        explicit GltfModel(const std::string& resourceName);
        ~GltfModel() = default;

        void draw(std::size_t instanceCount = 1) {
            if (mesh_ != nullptr) {
                mesh_->draw(static_cast<GLsizei>(instanceCount));
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
