#pragma once

#include <glad/gl.h>

#include "SatisfactorySave/Pak/Serialization/Texture2D.h"

namespace Satisfactory3DMap {
    GLuint makeOpenGLTexture(const Texture2D& tex);
} // namespace Satisfactory3DMap
