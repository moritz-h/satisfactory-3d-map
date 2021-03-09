#ifndef SATISFACTORY3DMAP_MAINWINDOW_H
#define SATISFACTORY3DMAP_MAINWINDOW_H

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

    class MainWindow {
    public:
        MainWindow();
        ~MainWindow();

        void run();

    private:
        GLFWwindow* window_;
        bool running_;

        static void initGLFW();
        static void terminateGLFW();

        static inline int glfwReferenceCounter_ = 0;
    };
} // namespace Satisfactory3DMap

#endif // SATISFACTORY3DMAP_MAINWINDOW_H
