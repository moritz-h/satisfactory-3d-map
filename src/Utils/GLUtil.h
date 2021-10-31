#ifndef SATISFACTORY3DMAP_GLUTIL_H
#define SATISFACTORY3DMAP_GLUTIL_H

#include <iostream>
#include <string>

#include <glad/gl.h>

namespace Satisfactory3DMap {
    class GLUtil {
    public:
        static void GLAPIENTRY OpenGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
            [[maybe_unused]] GLsizei length, const GLchar* message, [[maybe_unused]] const void* userParam) {
            // clang-format off
            std::cerr << (type == GL_DEBUG_TYPE_ERROR ? "[OpenGL Error]" : "[OpenGL Debug]")
                      << "  Source: " << getSource(source)
                      << "  Type: " << getType(type)
                      << "  Severity: " << getSeverity(severity)
                      << "  Id: " << id
                      << "  Message: " << message
                      << std::endl;
            // clang-format on
        }

    private:
        static std::string getSource(GLenum source) {
            switch (source) {
                case GL_DEBUG_SOURCE_API:
                    return "API";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                    return "WINDOW_SYSTEM";
                case GL_DEBUG_SOURCE_SHADER_COMPILER:
                    return "SHADER_COMPILER";
                case GL_DEBUG_SOURCE_THIRD_PARTY:
                    return "THIRD_PARTY";
                case GL_DEBUG_SOURCE_APPLICATION:
                    return "APPLICATION";
                case GL_DEBUG_SOURCE_OTHER:
                    return "OTHER";
                default:
                    return "UNKNOWN";
            }
        }

        static std::string getType(GLenum type) {
            switch (type) {
                case GL_DEBUG_TYPE_ERROR:
                    return "ERROR";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                    return "DEPRECATED_BEHAVIOR";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                    return "UNDEFINED_BEHAVIOR";
                case GL_DEBUG_TYPE_PORTABILITY:
                    return "PORTABILITY";
                case GL_DEBUG_TYPE_PERFORMANCE:
                    return "PERFORMANCE";
                case GL_DEBUG_TYPE_MARKER:
                    return "MARKER";
                case GL_DEBUG_TYPE_PUSH_GROUP:
                    return "PUSH_GROUP";
                case GL_DEBUG_TYPE_POP_GROUP:
                    return "POP_GROUP";
                case GL_DEBUG_TYPE_OTHER:
                    return "OTHER";
                default:
                    return "UNKNOWN";
            }
        }

        static std::string getSeverity(GLenum severity) {
            switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:
                    return "HIGH";
                case GL_DEBUG_SEVERITY_MEDIUM:
                    return "MEDIUM";
                case GL_DEBUG_SEVERITY_LOW:
                    return "LOW";
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    return "NOTIFICATION";
                default:
                    return "UNKNOWN";
            }
        }
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_GLUTIL_H
