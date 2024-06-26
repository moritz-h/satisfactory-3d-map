#include "Texture.h"

GLuint Satisfactory3DMap::makeOpenGLTexture(const SatisfactorySave::Texture2D& tex) {
    const auto& runningPlatformData = tex.runningPlatformData();
    const auto& pixelFormat = runningPlatformData.PixelFormatString;
    const auto& mips = runningPlatformData.Mips;

    bool isCompressed = false;
    GLenum internalformat = 0;
    GLenum format = 0;
    GLenum type = 0;
    if (pixelFormat == "PF_DXT1") {
        isCompressed = true;
        internalformat = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;
        format = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;
    } else if (pixelFormat == "PF_BC5") {
        isCompressed = true;
        internalformat = GL_COMPRESSED_RG_RGTC2;
        format = GL_COMPRESSED_RG_RGTC2;
    } else if (pixelFormat == "PF_B8G8R8A8") {
        internalformat = GL_RGBA8;
        format = GL_BGRA;
        type = GL_UNSIGNED_BYTE;
    } else {
        throw std::runtime_error("Unknown PixelFormatString: " + pixelFormat);
    }

    GLint maxLevel = static_cast<GLint>(mips.size()) - 3; // stop at 4x4

    GLuint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, maxLevel);

    glTextureStorage2D(texture, maxLevel + 1, internalformat, runningPlatformData.SizeX, runningPlatformData.SizeY);
    for (int lvl = 0; lvl <= maxLevel; lvl++) {
        if (isCompressed) {
            glCompressedTextureSubImage2D(texture, lvl, 0, 0, mips[lvl].SizeX, mips[lvl].SizeY, format,
                static_cast<GLsizei>(mips[lvl].BulkData.data.size()), mips[lvl].BulkData.data.data());
        } else {
            glTextureSubImage2D(texture, lvl, 0, 0, mips[lvl].SizeX, mips[lvl].SizeY, format, type,
                mips[lvl].BulkData.data.data());
        }
    }

    return texture;
}
