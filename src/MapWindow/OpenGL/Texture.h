#ifndef SATISFACTORY3DMAP_TEXTURE_H
#define SATISFACTORY3DMAP_TEXTURE_H

#include <glad/gl.h>

#include "GameTypes/Serialization/Texture2D.h"

namespace Satisfactory3DMap {
    GLuint makeOpenGLTexture(const Texture2D& tex);
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_TEXTURE_H
