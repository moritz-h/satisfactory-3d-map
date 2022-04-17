#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "DataView/DataView.h"

namespace Satisfactory3DMap {

    class ModelRenderer {
    public:
        explicit ModelRenderer(std::shared_ptr<DataView> dataView);
        ~ModelRenderer() = default;

        void render(const glm::mat4& projMx, const glm::mat4& viewMx, int selectedId);

        bool& wireframe() {
            return wireframe_;
        };

    protected:
        std::shared_ptr<DataView> dataView_;

        std::unique_ptr<glowl::GLSLProgram> pakShader_;
        std::unique_ptr<glowl::GLSLProgram> shader_;
        std::unique_ptr<glowl::GLSLProgram> splineShader_;

        bool wireframe_;
    };
} // namespace Satisfactory3DMap
