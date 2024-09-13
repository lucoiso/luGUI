// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

module luGUI.UserInterface.Window;

import Timer.Manager;
import RenderCore.Renderer;
import RenderCore.Runtime.Command;
import RenderCore.Utils.Helpers;
import luGUI.Integrations.ImGuiOverlay;
import luGUI.Integrations.ImGuiVulkanBackend;
import luGUI.Integrations.ImGuiGLFWBackend;
import luGUI.Integrations.GLFWCallbacks;
import luGUI.Integrations.GLFWHandler;

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
        SetCallbacks();

        std::binary_semaphore Sync { 1U };

        s_TimerManager.SetupThread("Main Thread");

        s_TimerManager.SetTimer(std::chrono::nanoseconds { 0 },
                                [&]
                                {
                                    [[maybe_unused]] bool const _ = RenderCore::Renderer::Initialize(m_GLFWHandler.GetWindow());
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

void Window::SetCallbacks()
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

    RenderCore::Renderer::SetOnInitializeCallback([this]
    {
        InitializeImGuiContext(m_GLFWHandler.GetWindow(),
                               HasFlag(m_Flags, InitializationFlags::ENABLE_DOCKING),
                               HasFlag(m_Flags, InitializationFlags::ENABLE_VIEWPORTS));

        Control::Initialize();
    });

    RenderCore::Renderer::SetOnRefreshCallback([this]
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
    });

    RenderCore::Renderer::SetOnDrawCallback([this]
    {
        if (!IsImGuiInitialized())
        {
            return;
        }

        DrawImGuiFrame(this);
    });

    RenderCore::Renderer::SetOnShutdownCallback([this]
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
                RenderCore::Renderer::DrawFrame(m_GLFWHandler.GetWindow(), DeltaTime);
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
