// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/vulkan-renderer

module;

#ifdef GLFW_INCLUDE_VULKAN
    #undef GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

export module luGUI.Integrations.GLFWCallbacks;

namespace luGUI
{
    export [[nodiscard]] bool IsResizingMainWindow();
    void                      GLFWWindowCloseRequestedCallback(GLFWwindow *);
    void                      GLFWWindowResizedCallback(GLFWwindow *, std::int32_t, std::int32_t);
    export void               GLFWErrorCallback(std::int32_t, char const *);
    void                      GLFWKeyCallback(GLFWwindow *, std::int32_t, std::int32_t, std::int32_t, std::int32_t);
    export void               GLFWCursorPositionCallback(GLFWwindow *, double, double);
    void                      GLFWCursorScrollCallback(GLFWwindow *, double, double);
    export void               InstallGLFWCallbacks(GLFWwindow *, bool);
    export void               SetViewportControlsCamera(bool);
    export [[nodiscard]] bool ViewportControlsCamera();
    export void               SetViewportHovering(bool);
} // namespace luGUI
