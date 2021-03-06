#include "MainWindow.h"

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

static constexpr int initWindowSizeWidth = 1280;
static constexpr int initWindowSizeHeight = 800;
static constexpr int openGLVersionMajor = 4;
static constexpr int openGLVersionMinor = 3;
static constexpr char imguiGlslVersion[] = "#version 430";
static constexpr char title[] = "Satisfactory Save Game";

SatisfactorySaveGame::MainWindow::MainWindow() : window_(nullptr), running_(false) {
    MainWindow::initGLFW();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(initWindowSizeWidth, initWindowSizeHeight, title, nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("GLFW window creation failed!");
    }

    glfwMakeContextCurrent(window_);

    int gladGLVersion = gladLoadGL(glfwGetProcAddress);
    if (gladGLVersion == 0) {
        throw std::runtime_error("Failed to initialize OpenGL context!");
    }

    if (gladGLVersion < GLAD_MAKE_VERSION(openGLVersionMajor, openGLVersionMinor)) {
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
    ImGui_ImplOpenGL3_Init(imguiGlslVersion);
}

SatisfactorySaveGame::MainWindow::~MainWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window_);
    MainWindow::terminateGLFW();
}

void SatisfactorySaveGame::MainWindow::run() {
    if (running_) {
        throw std::runtime_error("MainWindow is already running!");
    }
    running_ = true;
    while (!glfwWindowShouldClose(window_)) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(
            nullptr, ImGuiDockNodeFlags_NoDockingInCentralNode | ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        ImGui::SetNextWindowPos(ImVec2(10.0, 40.0), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(200.0, 200.0), ImGuiCond_Once);

        ImGui::Begin(title);
        ImGui::Text("Hello imgui!");
        ImGui::End();

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
    running_ = false;
}

void SatisfactorySaveGame::MainWindow::initGLFW() {
    if (SatisfactorySaveGame::MainWindow::glfwReferenceCounter_ <= 0) {
        glfwSetErrorCallback([](int error_code, const char* description) {
            std::cerr << "GLFW Error (" << error_code << "): " << description << std::endl;
        });
        if (!glfwInit()) {
            throw std::runtime_error("GLFW init failed!");
        }
    }
    SatisfactorySaveGame::MainWindow::glfwReferenceCounter_++;
}

void SatisfactorySaveGame::MainWindow::terminateGLFW() {
    SatisfactorySaveGame::MainWindow::glfwReferenceCounter_--;
    if (SatisfactorySaveGame::MainWindow::glfwReferenceCounter_ <= 0) {
        glfwTerminate();
        SatisfactorySaveGame::MainWindow::glfwReferenceCounter_ = 0;
    }
}
