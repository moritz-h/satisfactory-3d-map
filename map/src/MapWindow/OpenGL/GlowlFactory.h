#pragma once

#include <memory>

#include <glowl/glowl.h>

#include "SatisfactorySave/GameTypes/UE/Engine/Engine/StaticMesh.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap {
    std::shared_ptr<glowl::Mesh> makeGlowlMesh(const s::UStaticMesh& mesh);
} // namespace Satisfactory3DMap
