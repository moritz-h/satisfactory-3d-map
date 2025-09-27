#include "Texture.h"

#include "SatisfactorySave/GameTypes/Properties/BoolProperty.h"

Satisfactory3DMap::OGLTexture2D::OGLTexture2D(const s::UTexture2D& ueTex) {
    bool srgb = true;
    try {
        srgb = ueTex.Properties.get<SatisfactorySave::BoolProperty>("SRGB").getValue();
    } catch (...) {}

    const auto& runningPlatformData = ueTex.RunningPlatformData;
    const auto& pixelFormat = runningPlatformData.PixelFormatString;
    const auto& mips = runningPlatformData.Mips;

    if (mips.empty()) {
        throw std::runtime_error("No texture mipmap!");
    }

    sizeX_ = mips[0].SizeX;
    sizeY_ = mips[0].SizeY;

    bool isCompressed = false;
    GLenum internalformat = 0;
    GLenum format = 0;
    GLenum type = 0;
    if (pixelFormat == "PF_DXT1") {
        isCompressed = true;
        if (srgb) {
            internalformat = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;
            format = GL_COMPRESSED_SRGB_S3TC_DXT1_EXT;
        } else {
            internalformat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
            format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
        }
    } else if (pixelFormat == "PF_DXT5") {
        isCompressed = true;
        if (srgb) {
            internalformat = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
            format = GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
        } else {
            internalformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        }
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

    GLint numLevels = 0;
    for (const auto& mip : mips) {
        // stop at 4x4
        if (mip.SizeX < 4 || mip.SizeY < 4) {
            break;
        }
        numLevels++;
    }
    if (numLevels == 0) {
        throw std::runtime_error("No mip levels!");
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &texture_);

    glTextureParameteri(texture_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture_, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteri(texture_, GL_TEXTURE_MAX_LEVEL, numLevels - 1);

    glTextureStorage2D(texture_, numLevels, internalformat, sizeX_, sizeY_);
    for (int lvl = 0; lvl < numLevels; lvl++) {
        if (isCompressed) {
            glCompressedTextureSubImage2D(texture_, lvl, 0, 0, mips[lvl].SizeX, mips[lvl].SizeY, format,
                static_cast<GLsizei>(mips[lvl].BulkData.data.size()), mips[lvl].BulkData.data.data());
        } else {
            glTextureSubImage2D(texture_, lvl, 0, 0, mips[lvl].SizeX, mips[lvl].SizeY, format, type,
                mips[lvl].BulkData.data.data());
        }
    }
}

Satisfactory3DMap::OGLTexture2D::~OGLTexture2D() {
    glDeleteTextures(1, &texture_);
}
