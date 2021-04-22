#include "BaseWindow.h"

#include <iostream>
#include <stdexcept>
#include <utility>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Utils/GLUtil.h"

namespace {
    // GLFW mods parameter is not platform independent, see https://github.com/glfw/glfw/issues/1630.
    int fixKeyboardMods(int mods, int key, int action) {
        if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
            return (action == GLFW_RELEASE) ? mods & (~GLFW_MOD_SHIFT) : mods | GLFW_MOD_SHIFT;
        }
        if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
            return (action == GLFW_RELEASE) ? mods & (~GLFW_MOD_CONTROL) : mods | GLFW_MOD_CONTROL;
        }
        if (key == GLFW_KEY_LEFT_ALT || key == GLFW_KEY_RIGHT_ALT) {
            return (action == GLFW_RELEASE) ? mods & (~GLFW_MOD_ALT) : mods | GLFW_MOD_ALT;
        }
        if (key == GLFW_KEY_LEFT_SUPER || key == GLFW_KEY_RIGHT_SUPER) {
            return (action == GLFW_RELEASE) ? mods & (~GLFW_MOD_SUPER) : mods | GLFW_MOD_SUPER;
        }
        return mods;
    }
} // namespace

Satisfactory3DMap::BaseWindow::BaseWindow(std::string title, int width, int height, int openGLVersionMajor,
    int openGLVersionMinor, std::string imguiGlslVersion)
    : title_(std::move(title)),
      initWindowSizeWidth_(width),
      initWindowSizeHeight_(height),
      openGLVersionMajor_(openGLVersionMajor),
      openGLVersionMinor_(openGLVersionMinor),
      imguiGlslVersion_(std::move(imguiGlslVersion)),
      window_(nullptr),
      running_(false),
      width_(width),
      height_(height) {
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

    // Set OpenGL error callback
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLUtil::OpenGLMessageCallback, nullptr);

    // Setup GLFW callbacks
    glfwSetWindowUserPointer(window_, this);

    glfwSetWindowRefreshCallback(window_, [](GLFWwindow* window) {
        static_cast<BaseWindow*>(glfwGetWindowUserPointer(window))->draw();
        glfwSwapBuffers(window);
    });

    glfwSetFramebufferSizeCallback(window_, []([[maybe_unused]] GLFWwindow* window, int width, int height) {
        // Update viewport
        glViewport(0, 0, width, height);
    });

    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
        // Assume Win32 or X11 system. According to GLFW docs window size to framebuffer size is 1:1 on this systems. We
        // use window and framebuffer size as the same value now. But here at least we check if they are really the
        // same, to throw at least a meaningful exception in case anybody is using a different system or GLFW behavior
        // changes in future.
        int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
        if (width != framebufferWidth || height != framebufferHeight) {
            throw std::runtime_error("Window size and framebuffer size are not the same! "
                                     "You are probably using an unsupported system.");
        }
        auto w = static_cast<BaseWindow*>(glfwGetWindowUserPointer(window));
        w->width_ = width;
        w->height_ = height;
        w->resizeEvent(width, height);
    });
    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        mods = fixKeyboardMods(mods, key, action);
        if (!ImGui::GetIO().WantCaptureKeyboard) {
            static_cast<BaseWindow*>(glfwGetWindowUserPointer(window))->keyEvent(key, scancode, action, mods);
        }
    });
    glfwSetCharCallback(window_, [](GLFWwindow* window, unsigned int codepoint) {
        if (!ImGui::GetIO().WantTextInput) {
            static_cast<BaseWindow*>(glfwGetWindowUserPointer(window))->charEvent(codepoint);
        }
    });
    glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods) {
        if (!ImGui::GetIO().WantCaptureMouse) {
            static_cast<BaseWindow*>(glfwGetWindowUserPointer(window))->mouseButtonEvent(button, action, mods);
        }
    });
    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xpos, double ypos) {
        if (!ImGui::GetIO().WantCaptureMouse) {
            static_cast<BaseWindow*>(glfwGetWindowUserPointer(window))->mouseMoveEvent(xpos, ypos);
        }
    });
    glfwSetScrollCallback(window_, [](GLFWwindow* window, double xoffset, double yoffset) {
        if (!ImGui::GetIO().WantCaptureMouse) {
            static_cast<BaseWindow*>(glfwGetWindowUserPointer(window))->mouseScrollEvent(xoffset, yoffset);
        }
    });
    glfwSetDropCallback(window_, [](GLFWwindow* window, int path_count, const char* paths[]) {
        std::vector<std::string> path_list;
        for (int i = 0; i < path_count; i++) {
            path_list.emplace_back(std::string(paths[i]));
        }
        static_cast<BaseWindow*>(glfwGetWindowUserPointer(window))->dropEvent(path_list);
    });

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
        draw();
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
    running_ = false;
}

void Satisfactory3DMap::BaseWindow::draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
