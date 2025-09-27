#pragma once

#include <glad/gl.h>

#include "SatisfactorySave/GameTypes/UE/Engine/Engine/Texture2D.h"

namespace s = SatisfactorySave;

namespace Satisfactory3DMap {
    class OGLTexture2D {
    public:
        explicit OGLTexture2D(const s::UTexture2D& ueTex);
        ~OGLTexture2D();

        [[nodiscard]] GLuint name() const {
            return texture_;
        }

        inline void bindTexture() const {
            glBindTexture(GL_TEXTURE_2D, texture_);
        }

        [[nodiscard]] inline int32_t sizeX() const {
            return sizeX_;
        }

        [[nodiscard]] inline int32_t sizeY() const {
            return sizeY_;
        }

    protected:
        GLuint texture_ = 0;
        int32_t sizeX_ = 0;
        int32_t sizeY_ = 0;
    };
} // namespace Satisfactory3DMap
