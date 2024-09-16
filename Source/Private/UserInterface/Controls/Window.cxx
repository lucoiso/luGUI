// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include <stb_image.h>

#ifdef GLFW_INCLUDE_VULKAN
    #undef GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#ifdef _WIN32
    #undef APIENTRY
    #define GLFW_EXPOSE_NATIVE_WIN32
#endif // _WIN32
#ifdef __APPLE__
    #define GLFW_EXPOSE_NATIVE_COCOA
#endif // __APPLE__
#include <imgui_internal.h>
#include <GLFW/glfw3native.h>

module luGUI.UserInterface.Controls.Window;

import Timer.Manager;
import RenderCore.Renderer;
import RenderCore.Runtime.Command;
import RenderCore.Runtime.Instance;
import RenderCore.Runtime.SwapChain;
import RenderCore.Runtime.Device;
import RenderCore.Utils.Helpers;
import RenderCore.Utils.EnumHelpers;
import RenderCore.Types.Allocation;
import luGUI.Integrations.ImGuiOverlay;
import luGUI.Integrations.ImGuiVulkanBackend;
import luGUI.Integrations.ImGuiGLFWBackend;
import luGUI.Integrations.GLFWCallbacks;
import luGUI.Integrations.GLFWHandler;
import luGUI.UserInterface.Singletons.ImageManager;
import luGUI.UserInterface.Singletons.FontManager;

using namespace luGUI;

Timer::Manager s_TimerManager {};

Window::Window()
    : Control(nullptr)
{
}

bool Window::Initialize(std::uint16_t const Width, std::uint16_t const Height, strzilla::string_view const Title, InitializationFlags const Flags)
{
    if (RenderCore::Renderer::IsInitialized())
    {
        return false;
    }

    m_Title  = Title;
    m_Width  = Width;
    m_Height = Height;
    m_Flags  = Flags;

    if (m_GLFWHandler.Initialize(m_Width, m_Height, m_Title, m_Flags))
    {
        SetRendererRequiredCallbacks();

        std::binary_semaphore Sync { 1U };

        s_TimerManager.SetupThread("Main Thread");

        s_TimerManager.SetTimer(std::chrono::nanoseconds { 0 },
                                [&]
                                {
                                    [[maybe_unused]] bool const _ = RenderCore::Renderer::Initialize();
                                    Sync.release();
                                    Draw();
                                });

        s_TimerManager.SetActive(true);
        Sync.acquire();

        return true;
    }

    return false;
}

void Window::RequestClose()
{
    m_PendingClose = true;
}

void Window::Shutdown()
{
    std::binary_semaphore Sync { 1U };

    s_TimerManager.SetTimer(std::chrono::nanoseconds { 0 },
                            [&]
                            {
                                if (RenderCore::Renderer::IsInitialized())
                                {
                                    ImageManager::Get().UnregisterAllTextures();
                                    FontManager::Get().UnregisterAllFonts();
                                    RenderCore::Renderer::Shutdown();
                                }

                                Sync.release();
                            });

    Sync.acquire();

    if (IsOpen())
    {
        m_GLFWHandler.Shutdown();
    }

    s_TimerManager.ClearTimers();
    s_TimerManager.SetActive(false);
}

bool Window::IsOpen() const
{
    return m_GLFWHandler.IsOpen();
}

void Window::SetIcon(strzilla::string_view const &Path) const
{
    if (std::empty(Path))
    {
        return;
    }

    GLFWimage Icon;
    Icon.pixels = stbi_load(std::data(Path), &Icon.width, &Icon.height, nullptr, 4);
    glfwSetWindowIcon(m_GLFWHandler.GetWindow(), 1, &Icon);
    stbi_image_free(Icon.pixels);
}

void Window::SetPosition(std::int32_t const X, std::int32_t const Y) const
{
    glfwSetWindowPos(m_GLFWHandler.GetWindow(), X, Y);
}

void Window::SetTitle(strzilla::string_view const &Title) const
{
    glfwSetWindowTitle(m_GLFWHandler.GetWindow(), std::data(Title));
}

strzilla::string Window::GetTitle() const
{
    return strzilla::string { glfwGetWindowTitle(m_GLFWHandler.GetWindow()) };
}

#ifdef _WIN32
void Window::SetAsChildOf(::HWND const ParentHandle)
{
    GLFWwindow* const MyWindow = m_GLFWHandler.GetWindow();
    ::HWND const MyHandle = glfwGetWin32Window(MyWindow);

    ::SetParent(MyHandle, ParentHandle);

    ::LONG Style = ::GetWindowLong(MyHandle, GWL_STYLE);
    Style &= ~WS_POPUP;
    Style |= WS_CHILDWINDOW;
    ::SetWindowLong(MyHandle, GWL_STYLE, Style);

    ::ShowWindow(MyHandle, SW_SHOW);
    ::ShowWindow(ParentHandle, SW_SHOW);

    ::SetWindowLongPtrW(MyHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ImGuiGLFWWndProc));
}
#endif // _WIN32

void Window::PollEvents()
{
    if (!IsOpen())
    {
        return;
    }

    glfwWaitEvents();

    if (auto *MainWindow = m_GLFWHandler.GetWindow();
        IsResizingMainWindow() && glfwGetMouseButton(MainWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        constexpr double DoubleMin = std::numeric_limits<double>::min();

        static double CursorX = DoubleMin;
        static double CursorY = DoubleMin;

        if (CursorX == DoubleMin || CursorY == DoubleMin)
        {
            glfwGetCursorPos(MainWindow, &CursorX, &CursorY);
        }

        double CursorPosX = CursorX;
        double CursorPosY = CursorY;
        glfwGetCursorPos(MainWindow, &CursorPosX, &CursorPosY);

        if (CursorPosX != DoubleMin || CursorPosY != DoubleMin)
        {
            GLFWCursorPositionCallback(MainWindow, CursorPosX, CursorPosY);
        }
    }

    RenderCore::DispatchQueue(RenderCore::Renderer::GetMainThreadDispatchQueue());

    if (m_PendingClose)
    {
        Shutdown();
    }
}

void Window::SetRendererRequiredCallbacks()
{
    RenderCore::SetOnCommandPoolResetCallbackCallback([](std::uint8_t const Index)
    {
        if (!IsImGuiInitialized())
        {
            return;
        }

        ImGuiVulkanResetThreadResources(Index);
    });

    RenderCore::SetOnCommandBufferRecordCallbackCallback([](VkCommandBuffer const &            CommandBuffer,
                                                            RenderCore::ImageAllocation const &SwapchainAllocation)
    {
        if (!IsImGuiInitialized())
        {
            return;
        }

        RecordImGuiCommandBuffer(CommandBuffer, SwapchainAllocation);
    });

    RenderCore::SetOnSurfaceCreationCallback([this](VkSurfaceKHR &Surface)
    {
        RenderCore::CheckVulkanResult(glfwCreateWindowSurface(RenderCore::GetInstance(), m_GLFWHandler.GetWindow(), nullptr, &Surface));
    });

    RenderCore::SetOnGetSurfacePropertiesCallback([this]
    {
        return GetSurfaceProperties(m_GLFWHandler.GetWindow());
    });

    RenderCore::SetOnGetAdditionalInstanceExtensions([]
    {
        return GetGLFWExtensions();
    });

    RenderCore::SetOnInitializeCallback([this]
    {
        InitializeImGuiContext(m_GLFWHandler.GetWindow(),
                               RenderCore::HasFlag(m_Flags, InitializationFlags::ENABLE_DOCKING),
                               RenderCore::HasFlag(m_Flags, InitializationFlags::ENABLE_VIEWPORTS));

        Control::Initialize();
    });

    RenderCore::SetOnRefreshCallback([this]
    {
        if (!IsImGuiInitialized())
        {
            return;
        }

        RenderCore::Renderer::DispatchToMainThread([]
        {
            ImGuiGLFWUpdateFrameBufferSizes();
        });

        RefreshResources();
        glfwPostEmptyEvent();
    });

    RenderCore::SetOnDrawCallback([this]
    {
        if (!IsImGuiInitialized())
        {
            return;
        }

        DrawImGuiFrame(this);
    });

    RenderCore::SetOnShutdownCallback([this]
    {
        DestroyChildren();
        ReleaseImGuiResources();
    });
}

void Window::Draw()
{
    if (!IsOpen() || m_PendingClose)
    {
        glfwPostEmptyEvent();
        return;
    }

    static auto LastTime    = std::chrono::high_resolution_clock::now();
    auto const  CurrentTime = std::chrono::high_resolution_clock::now();

    auto const     Milliseconds = std::chrono::duration<double, std::milli>(CurrentTime - LastTime).count();
    constexpr auto Denominator  = static_cast<double>(std::milli::den);

    double const &Interval = RenderCore::Renderer::GetFPSLimit();

    if (auto const DeltaTime = Milliseconds / Denominator;
        DeltaTime >= Interval)
    {
        LastTime = CurrentTime;

        if (RenderCore::Renderer::IsInitialized())
        {
            if (!IsResizingMainWindow())
            {
                RenderCore::Renderer::DrawFrame(DeltaTime);
            }
        }
        else
        {
            DestroyChildren();
        }
    }

    s_TimerManager.SetTimer(std::chrono::nanoseconds { 0 },
                            [&]
                            {
                                Draw();
                            });
}

void Window::Refresh()
{
    ImageManager::Get().RefreshTextures();
}
