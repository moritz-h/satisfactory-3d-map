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
            int openGLVersionMajor = 4, int openGLVersionMinor = 5, std::string imguiGlslVersion_ = "#version 450");
        ~BaseWindow();

        void run();

    protected:
        virtual void render(){};
        virtual void resizeEvent(int width, int height){};
        virtual void keyEvent(int key, int scancode, int action, int mods){};
        virtual void charEvent(unsigned int codepoint){};
        virtual void mouseButtonEvent(int button, int action, int mods){};
        virtual void mouseMoveEvent(double xpos, double ypos){};
        virtual void mouseScrollEvent(double xoffset, double yoffset){};
        virtual void dropEvent(const std::vector<std::string>& paths){};

        const std::string title_;
        const int initWindowSizeWidth_;
        const int initWindowSizeHeight_;
        const int openGLVersionMajor_;
        const int openGLVersionMinor_;
        const std::string imguiGlslVersion_;

        GLFWwindow* window_;
        bool running_;

        int width_;
        int height_;

    private:
        void draw();

        static void initGLFW();
        static void terminateGLFW();

        static inline int glfwReferenceCounter_ = 0;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_BASEWINDOW_H
