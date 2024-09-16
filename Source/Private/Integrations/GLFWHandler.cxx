// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/vulkan-renderer

module;

#ifdef GLFW_INCLUDE_VULKAN
    #undef GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

module luGUI.Integrations.GLFWHandler;

import luGUI.Integrations.GLFWCallbacks;
import RenderCore.Utils.EnumHelpers;

using namespace luGUI;

bool GLFWHandler::Initialize(std::uint16_t const         Width,
                             std::uint16_t const         Height,
                             strzilla::string_view const Title,
                             InitializationFlags const   Flags)
{
    if (!glfwInit() || !glfwVulkanSupported())
    {
        return false;
    }

    if (static bool GLFWErrorCallbacksSet = false;
        !GLFWErrorCallbacksSet)
    {
        glfwSetErrorCallback(&GLFWErrorCallback);
        GLFWErrorCallbacksSet = true;
    }

    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, RenderCore::HasFlag(Flags, InitializationFlags::MAXIMIZED) ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, RenderCore::HasFlag(Flags, InitializationFlags::HEADLESS) ? GLFW_FALSE : GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, RenderCore::HasFlag(Flags, InitializationFlags::WITHOUT_TITLEBAR) ? GLFW_FALSE : GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, RenderCore::HasFlag(Flags, InitializationFlags::ALWAYS_ON_TOP) ? GLFW_TRUE : GLFW_FALSE);

    m_Window = glfwCreateWindow(Width, Height, std::data(Title), nullptr, nullptr);

    if (GLFWmonitor *const Monitor = glfwGetPrimaryMonitor())
    {
        if (GLFWvidmode const *const VideoMode = glfwGetVideoMode(Monitor))
        {
            std::int32_t const WinPosX = (VideoMode->width - Width) / 2;
            std::int32_t const WinPosY = (VideoMode->height - Height) / 2;

            glfwSetWindowPos(m_Window, WinPosX, WinPosY);
        }
    }

    InstallGLFWCallbacks(m_Window, true);

    return m_Window != nullptr;
}

void GLFWHandler::Shutdown()
{
    glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
    glfwDestroyWindow(m_Window);
    m_Window = nullptr;
    glfwTerminate();
}

[[nodiscard]] GLFWwindow *GLFWHandler::GetWindow() const
{
    return m_Window;
}

[[nodiscard]] bool GLFWHandler::IsOpen() const
{
    return m_Window != nullptr && glfwWindowShouldClose(m_Window) == GLFW_FALSE;
}
