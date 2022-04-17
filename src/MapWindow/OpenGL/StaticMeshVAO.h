#pragma once

#include <glad/gl.h>

#include "GameTypes/Serialization/StaticMesh.h"

namespace Satisfactory3DMap {

    class StaticMeshVAO {
    public:
        explicit StaticMeshVAO(const StaticMesh& mesh);
        ~StaticMeshVAO() = default;

        void draw(std::size_t instanceCount = 1);

    protected:
        GLuint vao_;
        int indices_;
    };
} // namespace Satisfactory3DMap
