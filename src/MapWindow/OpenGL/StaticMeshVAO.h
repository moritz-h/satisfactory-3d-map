#pragma once

#include <memory>

#include <glowl/glowl.h>

namespace Satisfactory3DMap {
    class StaticMesh;

    std::shared_ptr<glowl::Mesh> makeGlowlMesh(const StaticMesh& mesh);
} // namespace Satisfactory3DMap
