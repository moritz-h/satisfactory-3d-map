#pragma once

#include <glad/gl.h>

#include "SatisfactorySave/GameTypes/UE/Engine/Engine/Texture2D.h"

namespace Satisfactory3DMap {
    GLuint makeOpenGLTexture(const SatisfactorySave::UTexture2D& tex);
} // namespace Satisfactory3DMap
