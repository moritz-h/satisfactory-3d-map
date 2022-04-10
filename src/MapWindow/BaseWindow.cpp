#include "BaseWindow.h"

#include <stdexcept>
#include <utility>

#include <IconsFontAwesome5.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include "Utils/GLUtil.h"
#include "Utils/ResourceUtils.h"

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
      framebufferWidth_(0),
      framebufferHeight_(0),
      windowWidth_(0),
      windowHeight_(0),
      contentScale_(-1.0f) {
    BaseWindow::initGLFW();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLVersionMajor_);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLVersionMinor_);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    window_ = glfwCreateWindow(initWindowSizeWidth_, initWindowSizeHeight_, title_.c_str(), nullptr, nullptr);
    if (!window_) {
        BaseWindow::terminateGLFW();
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
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(
        [](GLenum source, GLenum type, GLuint id, GLenum severity, [[maybe_unused]] GLsizei length,
            const GLchar* message, const void* userParam) {
            const std::string fmt = "[OpenGL]  Source: {}  Type: {}  Severity: {}  Id: {}  Message: {}";
            const auto sourceName = GLUtil::getGlDebugSourceName(source);
            const auto typeName = GLUtil::getGlDebugTypeName(type);
            const auto severityName = GLUtil::getGlDebugSeverityName(severity);

            spdlog::level::level_enum level = spdlog::level::warn;
            if (severity == GL_DEBUG_SEVERITY_HIGH) {
                level = spdlog::level::err;
            } else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
                level = spdlog::level::debug;
            }

            spdlog::log(level, fmt, sourceName, typeName, severityName, id, message);
        },
        nullptr);
    // ignore notifications
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    // The initial size above is only a hint for the window manager, but no guarantied window size. Further the window
    // size can be adjusted by DPI scaling on some systems. This initial resize will not be caught by the callback
    // events. Therefore, here do an initial size query.
    glfwGetWindowSize(window_, &windowWidth_, &windowHeight_);
    glfwGetFramebufferSize(window_, &framebufferWidth_, &framebufferHeight_);

    // Setup GLFW callbacks
    glfwSetWindowUserPointer(window_, this);

    glfwSetWindowRefreshCallback(window_, [](GLFWwindow* window) {
        static_cast<BaseWindow*>(glfwGetWindowUserPointer(window))->draw();
        glfwSwapBuffers(window);
    });

    glfwSetFramebufferSizeCallback(window_, []([[maybe_unused]] GLFWwindow* window, int width, int height) {
        auto w = static_cast<BaseWindow*>(glfwGetWindowUserPointer(window));
        w->framebufferWidth_ = width;
        w->framebufferHeight_ = height;
    });
    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
        auto w = static_cast<BaseWindow*>(glfwGetWindowUserPointer(window));
        w->windowWidth_ = width;
        w->windowHeight_ = height;
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
        path_list.reserve(path_count);
        for (int i = 0; i < path_count; i++) {
            path_list.emplace_back(std::string(paths[i]));
        }
        static_cast<BaseWindow*>(glfwGetWindowUserPointer(window))->dropEvent(path_list);
    });

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavNoCaptureKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init(imguiGlslVersion_.c_str());

    validateImGuiScale(); // Scale update includes font and style setup.
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
        return;
    }
    running_ = true;
    while (!glfwWindowShouldClose(window_)) {
        draw();
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
    running_ = false;
}

void Satisfactory3DMap::BaseWindow::validateImGuiScale() {
    float xscale, yscale;
    glfwGetWindowContentScale(window_, &xscale, &yscale);

    // Different x and y scaling is not handled
    const float scale = (xscale + yscale) * 0.5f;

    if (contentScale_ != scale) {
        // Setup fonts
        auto& io = ImGui::GetIO();
        io.Fonts->Clear();

        auto fontText = getBinaryResource("fonts/Roboto-Medium.ttf");
        ImFontConfig configText;
        configText.FontDataOwnedByAtlas = false;
        io.Fonts->AddFontFromMemoryTTF(fontText.data(), static_cast<int>(fontText.size()), 13.0f * scale, &configText);

        auto fontIcons = getBinaryResource("fonts/Font Awesome 5 Free-Solid-900.otf");
        ImFontConfig configIcons;
        configIcons.FontDataOwnedByAtlas = false;
        configIcons.MergeMode = true;
        configIcons.PixelSnapH = true;
        const ImWchar iconsRanges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
        io.Fonts->AddFontFromMemoryTTF(fontIcons.data(), static_cast<int>(fontIcons.size()), 15.0f * scale,
            &configIcons, iconsRanges);

        ImGui_ImplOpenGL3_DestroyFontsTexture();
        ImGui_ImplOpenGL3_CreateFontsTexture();

        // Setup style
        ImGui::GetStyle() = ImGuiStyle();
        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(scale);

        contentScale_ = scale;
    }
}

void Satisfactory3DMap::BaseWindow::draw() {
    validateImGuiScale();

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
            spdlog::error("GLFW Error ({}): {}", error_code, description);
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
