#include "Texture.h"

#include "SatisfactorySave/GameTypes/Properties/BoolProperty.h"

namespace {
    void createTexture(GLuint& tex, GLint maxLevel) {
        glCreateTextures(GL_TEXTURE_2D, 1, &tex);

        glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(tex, GL_TEXTURE_BASE_LEVEL, 0);
        glTextureParameteri(tex, GL_TEXTURE_MAX_LEVEL, maxLevel);
    }

    uint32_t decodeZCurve(uint32_t idx) {
        uint32_t x = idx & 0x55555555;
        x = (x | (x >> 1)) & 0x33333333;
        x = (x | (x >> 2)) & 0x0F0F0F0F;
        x = (x | (x >> 4)) & 0x00FF00FF;
        x = (x | (x >> 8)) & 0x0000FFFF;
        return x;
    }
} // namespace

Satisfactory3DMap::OGLTexture2D::OGLTexture2D(const s::UTexture2D& ueTex) {
    bool srgb = true;
    try {
        srgb = ueTex.Properties.get<SatisfactorySave::BoolProperty>("SRGB").getValue();
    } catch (...) {}

    const auto& runningPlatformData = ueTex.RunningPlatformData;
    const auto& pixelFormat = runningPlatformData.PixelFormatString;

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

    if (!runningPlatformData.bIsVirtual) {
        const auto& mips = runningPlatformData.Mips;

        if (mips.empty()) {
            throw std::runtime_error("No texture mipmap!");
        }

        sizeX_ = mips[0].SizeX;
        sizeY_ = mips[0].SizeY;

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

        createTexture(texture_, numLevels - 1);

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
    } else {
        const auto& vtData = ueTex.RunningPlatformData.VTData;

        if (vtData.NumLayers != 1 || vtData.WidthInBlocks != 1 || vtData.HeightInBlocks != 1 || vtData.NumMips < 1) {
            throw std::runtime_error("Virtual texture format not implemented!");
        }

        sizeX_ = static_cast<int32_t>(vtData.Width);
        sizeY_ = static_cast<int32_t>(vtData.Height);
        int32_t tileSize = static_cast<int32_t>(vtData.TileSize);
        int32_t tileBorderSize = static_cast<int32_t>(vtData.TileBorderSize);
        int32_t tileRawSize = tileSize + 2 * tileBorderSize;
        auto numTiles = vtData.TileOffsetData.at(0).Width;
        if (numTiles != vtData.TileOffsetData.at(0).Height || tileSize * numTiles != sizeX_ ||
            tileSize * numTiles != sizeY_) {
            throw std::runtime_error("Virtual texture format not implemented!");
        }

        createTexture(texture_, 0);
        glTextureStorage2D(texture_, 1, internalformat, sizeX_, sizeY_);

        GLuint bufTex;
        createTexture(bufTex, 0);
        glTextureStorage2D(bufTex, 1, internalformat, tileRawSize, tileRawSize);

        const char* data = vtData.Chunks.at(vtData.ChunkIndexPerMip.at(0)).BulkData.data.data();
        data += vtData.BaseOffsetPerMip.at(0);
        GLsizei dataOffset = vtData.TileDataOffsetPerLayer.at(0);

        for (uint32_t idx = 0; idx < numTiles * numTiles; idx++) {
            int32_t x = static_cast<int32_t>(decodeZCurve(idx));
            int32_t y = static_cast<int32_t>(decodeZCurve(idx >> 1));
            auto* tileData = data + idx * dataOffset;

            if (isCompressed) {
                glCompressedTextureSubImage2D(bufTex, 0, 0, 0, tileRawSize, tileRawSize, format, dataOffset, tileData);
            } else {
                glTextureSubImage2D(bufTex, 0, 0, 0, tileRawSize, tileRawSize, format, type, tileData);
            }
            glCopyImageSubData(bufTex, GL_TEXTURE_2D, 0, tileBorderSize, tileBorderSize, 0, texture_, GL_TEXTURE_2D, 0,
                x * tileSize, y * tileSize, 0, tileSize, tileSize, 1);
        }

        glDeleteTextures(1, &bufTex);
    }
}

Satisfactory3DMap::OGLTexture2D::~OGLTexture2D() {
    glDeleteTextures(1, &texture_);
}
