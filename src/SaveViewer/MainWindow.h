#ifndef SATISFACTORYSAVEGAME_MAINWINDOW_H
#define SATISFACTORYSAVEGAME_MAINWINDOW_H

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

namespace SatisfactorySaveGame {

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
} // namespace SatisfactorySaveGame

#endif // SATISFACTORYSAVEGAME_MAINWINDOW_H
