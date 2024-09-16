// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/vulkan-renderer

// Adapted from: https://github.com/ocornut/imgui/blob/docking/backends/imgui_impl_glfw.h

module;

#include <imgui.h>

#ifdef GLFW_INCLUDE_VULKAN
    #undef GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

export module luGUI.Integrations.ImGuiGLFWBackend;

import RenderCore.Types.SurfaceProperties;

namespace luGUI
{
    export struct ImGuiGLFWData
    {
        GLFWwindow *                                     Window {};
        double                                           Time {};
        GLFWwindow *                                     MouseWindow {};
        std::array<GLFWcursor *, ImGuiMouseCursor_COUNT> MouseCursors {};
        ImVec2                                           LastValidMousePos;
        std::array<GLFWwindow *, GLFW_KEY_LAST>          KeyOwnerWindows {};
        bool                                             InstalledCallbacks {};
        bool                                             CallbacksChainForAllWindows {};
        bool                                             WantUpdateMonitors {};

        GLFWwindowfocusfun PrevUserCallbackWindowFocus {};
        GLFWcursorposfun   PrevUserCallbackCursorPos {};
        GLFWcursorenterfun PrevUserCallbackCursorEnter {};
        GLFWmousebuttonfun PrevUserCallbackMousebutton {};
        GLFWscrollfun      PrevUserCallbackScroll {};
        GLFWkeyfun         PrevUserCallbackKey {};
        GLFWcharfun        PrevUserCallbackChar {};
        GLFWmonitorfun     PrevUserCallbackMonitor {};

        #ifdef _WIN32
        WNDPROC PrevWndProc {};
        #endif
    };

    export struct ImGuiGLFWViewportData
    {
        GLFWwindow * Window {};
        bool         WindowOwned {};
        std::int32_t IgnoreWindowPosEventFrame { -1 };
        std::int32_t IgnoreWindowSizeEventFrame { -1 };
        #ifdef _WIN32
        WNDPROC PrevWndProc {};
        #endif
    };

    export [[nodiscard]] ImGuiGLFWData *ImGuiGLFWGetBackendData();

    export [[nodiscard]] VkExtent2D                    GetFramebufferSize(GLFWwindow *);
    export [[nodiscard]] VkExtent2D                    GetWindowExtent(GLFWwindow *, VkSurfaceCapabilitiesKHR const &);
    export [[nodiscard]] std::vector<strzilla::string> GetGLFWExtensions();
    export [[nodiscard]] RenderCore::SurfaceProperties GetSurfaceProperties(GLFWwindow *);

    export bool ImGuiGLFWInitForVulkan(GLFWwindow *, bool);
    export void ImGuiGLFWShutdown();
    export void ImGuiGLFWNewFrame();
    export void ImGuiGLFWInstallCallbacks(GLFWwindow *);
    export void ImGuiGLFWRestoreCallbacks(GLFWwindow *);
    export void ImGuiGLFWSetCallbacksChainForAllWindows(bool);

    export void ImGuiGLFWUpdateFrameBufferSizes();
    export void ImGuiGLFWUpdateMouse();

    void ImGuiGLFWUpdateMonitors();
    void ImGuiGLFWInitPlatformInterface();
    void ImGuiGLFWShutdownPlatformInterface();
    void ImGuiGLFWWindowFocusCallback(GLFWwindow *, std::int32_t);
    void ImGuiGLFWCursorEnterCallback(GLFWwindow *, std::int32_t);
    void ImGuiGLFWCursorPosCallback(GLFWwindow *, double, double);
    void ImGuiGLFWMouseButtonCallback(GLFWwindow *, std::int32_t, std::int32_t, std::int32_t);
    void ImGuiGLFWScrollCallback(GLFWwindow *, double, double);
    void ImGuiGLFWKeyCallback(GLFWwindow *, std::int32_t, std::int32_t, std::int32_t, std::int32_t);
    void ImGuiGLFWCharCallback(GLFWwindow *, std::uint32_t);
    void ImGuiGLFWMonitorCallback(GLFWmonitor *, std::int32_t);

    #ifdef _WIN32
    export LRESULT CALLBACK ImGuiGLFWWndProc(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam);
    #endif // _WIN32
} // namespace luGUI
