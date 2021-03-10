#include "BaseWindow.h"

#include <iostream>
#include <utility>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Satisfactory3DMap::BaseWindow::BaseWindow(std::string title, int width, int height, int openGLVersionMajor,
    int openGLVersionMinor, std::string imguiGlslVersion)
    : title_(std::move(title)),
      initWindowSizeWidth_(width),
      initWindowSizeHeight_(height),
      openGLVersionMajor_(openGLVersionMajor),
      openGLVersionMinor_(openGLVersionMinor),
      imguiGlslVersion_(std::move(imguiGlslVersion)),
      window_(nullptr),
      running_(false) {
    BaseWindow::initGLFW();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLVersionMajor_);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLVersionMinor_);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(initWindowSizeWidth_, initWindowSizeHeight_, title_.c_str(), nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("GLFW window creation failed!");
    }

    glfwMakeContextCurrent(window_);

    int gladGLVersion = gladLoadGL(glfwGetProcAddress);
    if (gladGLVersion == 0) {
        throw std::runtime_error("Failed to initialize OpenGL context!");
    }

    if (gladGLVersion < GLAD_MAKE_VERSION(openGLVersionMajor_, openGLVersionMinor_)) {
        throw std::runtime_error("OpenGL context does not match requested version!");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init(imguiGlslVersion_.c_str());
}

Satisfactory3DMap::BaseWindow::~BaseWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window_);
    BaseWindow::terminateGLFW();
}

void Satisfactory3DMap::BaseWindow::run() {
    if (running_) {
        throw std::runtime_error("MainWindow is already running!");
    }
    running_ = true;
    while (!glfwWindowShouldClose(window_)) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
    running_ = false;
}

void Satisfactory3DMap::BaseWindow::initGLFW() {
    if (Satisfactory3DMap::BaseWindow::glfwReferenceCounter_ <= 0) {
        glfwSetErrorCallback([](int error_code, const char* description) {
            std::cerr << "GLFW Error (" << error_code << "): " << description << std::endl;
        });
        if (!glfwInit()) {
            throw std::runtime_error("GLFW init failed!");
        }
    }
    Satisfactory3DMap::BaseWindow::glfwReferenceCounter_++;
}

void Satisfactory3DMap::BaseWindow::terminateGLFW() {
    Satisfactory3DMap::BaseWindow::glfwReferenceCounter_--;
    if (Satisfactory3DMap::BaseWindow::glfwReferenceCounter_ <= 0) {
        glfwTerminate();
        Satisfactory3DMap::BaseWindow::glfwReferenceCounter_ = 0;
    }
}
