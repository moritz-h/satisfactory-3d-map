#ifndef SATISFACTORY3DMAP_BASEWINDOW_H
#define SATISFACTORY3DMAP_BASEWINDOW_H

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

namespace Satisfactory3DMap {

    class BaseWindow {
    public:
        explicit BaseWindow(std::string title = "Window", int width = 1280, int height = 800,
            int openGLVersionMajor = 4, int openGLVersionMinor = 3, std::string imguiGlslVersion_ = "#version 430");
        ~BaseWindow();

        void run();

    protected:
        virtual void render(){};

        const std::string title_;
        const int initWindowSizeWidth_;
        const int initWindowSizeHeight_;
        const int openGLVersionMajor_;
        const int openGLVersionMinor_;
        const std::string imguiGlslVersion_;

        GLFWwindow* window_;
        bool running_;

    private:
        static void initGLFW();
        static void terminateGLFW();

        static inline int glfwReferenceCounter_ = 0;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BASEWINDOW_H
