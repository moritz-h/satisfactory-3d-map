#pragma once

#include <memory>

#include <glowl/glowl.h>

namespace SatisfactorySave {
    class StaticMesh;
}

namespace Satisfactory3DMap {
    std::shared_ptr<glowl::Mesh> makeGlowlMesh(const SatisfactorySave::StaticMesh& mesh);
} // namespace Satisfactory3DMap
