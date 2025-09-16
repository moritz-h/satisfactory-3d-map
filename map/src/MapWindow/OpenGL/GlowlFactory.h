#pragma once

#include <memory>

#include <glowl/glowl.h>

namespace SatisfactorySave {
    class UStaticMesh;
}

namespace Satisfactory3DMap {
    std::shared_ptr<glowl::Mesh> makeGlowlMesh(const SatisfactorySave::UStaticMesh& mesh);
} // namespace Satisfactory3DMap
