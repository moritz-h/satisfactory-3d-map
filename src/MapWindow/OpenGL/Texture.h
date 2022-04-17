#pragma once

#include <glad/gl.h>

#include "GameTypes/Serialization/Texture2D.h"

namespace Satisfactory3DMap {
    GLuint makeOpenGLTexture(const Texture2D& tex);
} // namespace Satisfactory3DMap
