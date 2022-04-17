#pragma once

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
        virtual void keyEvent([[maybe_unused]] int key, [[maybe_unused]] int scancode, [[maybe_unused]] int action,
            [[maybe_unused]] int mods){};
        virtual void charEvent([[maybe_unused]] unsigned int codepoint){};
        virtual void mouseButtonEvent([[maybe_unused]] int button, [[maybe_unused]] int action,
            [[maybe_unused]] int mods){};
        virtual void mouseMoveEvent([[maybe_unused]] double xpos, [[maybe_unused]] double ypos){};
        virtual void mouseScrollEvent([[maybe_unused]] double xoffset, [[maybe_unused]] double yoffset){};
        virtual void dropEvent([[maybe_unused]] const std::vector<std::string>& paths){};

        const std::string title_;
        const int initWindowSizeWidth_;
        const int initWindowSizeHeight_;
        const int openGLVersionMajor_;
        const int openGLVersionMinor_;
        const std::string imguiGlslVersion_;

        GLFWwindow* window_;
        bool running_;

        int framebufferWidth_;
        int framebufferHeight_;
        int windowWidth_;
        int windowHeight_;

        float contentScale_;

    private:
        void validateImGuiScale();
        void draw();

        static void initGLFW();
        static void terminateGLFW();

        static inline int glfwReferenceCounter_ = 0;
    };
} // namespace Satisfactory3DMap
