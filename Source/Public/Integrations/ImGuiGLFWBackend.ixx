// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/vulkan-renderer

// Adapted from: https://github.com/ocornut/imgui/blob/docking/backends/imgui_impl_glfw.h

module;

#ifdef GLFW_INCLUDE_VULKAN
    #undef GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

export module luGUI.Integrations.ImGuiGLFWBackend;

import RenderCore.Types.SurfaceProperties;

namespace luGUI
{
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
} // namespace luGUI
