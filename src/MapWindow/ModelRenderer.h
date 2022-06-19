#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glowl/glowl.h>

#include "Config/BoolSetting.h"
#include "Config/Configuration.h"
#include "DataView/DataView.h"

namespace Satisfactory3DMap {

    class ModelRenderer {
    public:
        explicit ModelRenderer(const std::shared_ptr<Configuration>& config, std::shared_ptr<DataView> dataView);
        ~ModelRenderer() = default;

        void render(const glm::mat4& projMx, const glm::mat4& viewMx, int selectedId);

    protected:
        std::shared_ptr<DataView> dataView_;

        std::unique_ptr<glowl::GLSLProgram> pakShader_;
        std::unique_ptr<glowl::GLSLProgram> shader_;
        std::unique_ptr<glowl::GLSLProgram> splineShader_;

        std::shared_ptr<BoolSetting> wireframeSetting_;
    };
} // namespace Satisfactory3DMap
