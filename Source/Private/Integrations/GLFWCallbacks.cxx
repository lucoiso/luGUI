// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/vulkan-renderer

module;

module luGUI.Integrations.GLFWCallbacks;

import RenderCore.Renderer;
import RenderCore.Runtime.Scene;
import RenderCore.Types.RendererStateFlags;
import RenderCore.Types.Camera;
import RenderCore.Utils.EnumHelpers;
import luGUI.Integrations.ImGuiOverlay;
import luGUI.Integrations.ImGuiGLFWBackend;

using namespace luGUI;

bool g_IsPressingLeft = false;
bool g_IsPressingRight = false;
bool g_ViewportControlsCamera = false;
bool g_ViewportHovering = false;
bool g_CanMovementCamera    = false;
bool g_CanMovementWindow    = false;
bool g_IsResizingMainWindow = false;

bool luGUI::IsResizingMainWindow()
{
    return g_IsResizingMainWindow;
}

void luGUI::GLFWWindowCloseRequestedCallback(GLFWwindow *const Window)
{
    std::lock_guard const Lock { RenderCore::Renderer::GetMutex() };
    glfwSetWindowShouldClose(Window, GLFW_TRUE);
}

void luGUI::GLFWWindowResizedCallback([[maybe_unused]] GLFWwindow *const  Window,
                                           [[maybe_unused]] std::int32_t const Width,
                                           [[maybe_unused]] std::int32_t const Height)
{
    g_IsResizingMainWindow                    = true;
    constexpr RenderCore::RendererStateFlags RefreshFlags = RenderCore::RendererStateFlags::INVALID_SIZE | RenderCore::RendererStateFlags::PENDING_DEVICE_PROPERTIES_UPDATE;

    if (Width <= 0 || Height <= 0)
    {
        RenderCore::Renderer::AddStateFlag(RefreshFlags);
    }
    else
    {
        RenderCore::Renderer::RemoveStateFlag(RefreshFlags);
    }

    RenderCore::Renderer::RequestUpdateResources();
}

void luGUI::GLFWErrorCallback(std::int32_t const Error, char const *const Description)
{
    BOOST_LOG_TRIVIAL(error) << "[" << __func__ << "]: GLFW Error: " << Error << " - " << Description;
}

void luGUI::GLFWKeyCallback([[maybe_unused]] GLFWwindow *const  Window,
                                 std::int32_t const                  Key,
                                 [[maybe_unused]] std::int32_t const Scancode,
                                 std::int32_t const                  Action,
                                 [[maybe_unused]] std::int32_t const Mods)
{
    RenderCore::Camera &                 Camera               = RenderCore::GetCamera();
    RenderCore::CameraMovementStateFlags CurrentMovementState = Camera.GetCameraMovementStateFlags();

    if (!g_CanMovementCamera)
    {
        Camera.SetCameraMovementStateFlags(RenderCore::CameraMovementStateFlags::NONE);
        return;
    }

    if (Action == GLFW_PRESS)
    {
        switch (Key)
        {
            case GLFW_KEY_W:
                AddFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::FORWARD);
                break;
            case GLFW_KEY_S:
                AddFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::BACKWARD);
                break;
            case GLFW_KEY_A:
                AddFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::LEFT);
                break;
            case GLFW_KEY_D:
                AddFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::RIGHT);
                break;
            case GLFW_KEY_Q:
                AddFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::DOWN);
                break;
            case GLFW_KEY_E:
            case GLFW_KEY_SPACE:
                AddFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::UP);
                break;
            default:
                break;
        }
    }
    else if (Action == GLFW_RELEASE)
    {
        switch (Key)
        {
            case GLFW_KEY_W:
                RemoveFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::FORWARD);
                break;
            case GLFW_KEY_S:
                RemoveFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::BACKWARD);
                break;
            case GLFW_KEY_A:
                RemoveFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::LEFT);
                break;
            case GLFW_KEY_D:
                RemoveFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::RIGHT);
                break;
            case GLFW_KEY_Q:
                RemoveFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::DOWN);
                break;
            case GLFW_KEY_E:
            case GLFW_KEY_SPACE:
                RemoveFlags(CurrentMovementState, RenderCore::CameraMovementStateFlags::UP);
                break;
            default:
                break;
        }
    }

    Camera.SetCameraMovementStateFlags(CurrentMovementState);
}

static void MovementWindow(GLFWwindow *const Window, double const NewCursorPosX, double const NewCursorPosY)
{
    if (!g_CanMovementWindow)
    {
        return;
    }

    static double InitialCursorPosX = 0.0;
    static double InitialCursorPosY = 0.0;
    static bool   IsDragging        = false;

    if (glfwGetWindowAttrib(Window, GLFW_DECORATED) == GLFW_FALSE)
    {
        if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            if (!IsDragging)
            {
                glfwGetCursorPos(Window, &InitialCursorPosX, &InitialCursorPosY);
                IsDragging = true;
            }

            std::int32_t WindowX;
            std::int32_t WindowY;
            glfwGetWindowPos(Window, &WindowX, &WindowY);

            auto const NewPosX = WindowX + static_cast<std::int32_t>(NewCursorPosX - InitialCursorPosX);
            auto const NewPosY = WindowY + static_cast<std::int32_t>(NewCursorPosY - InitialCursorPosY);

            glfwSetWindowPos(Window, NewPosX, NewPosY);
        }
        else
        {
            IsDragging = false;
        }
    }
}

static void MovementCamera(GLFWwindow *const Window, double const NewCursorPosX, double const NewCursorPosY)
{
    if (!IsImGuiInitialized())
    {
        return;
    }

    static double LastCursorPosX = NewCursorPosX;
    static double LastCursorPosY = NewCursorPosY;

    float const OffsetX { static_cast<float>(LastCursorPosX - NewCursorPosX) };
    float const OffsetY { static_cast<float>(LastCursorPosY - NewCursorPosY) };

    if (g_CanMovementCamera)
    {
        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        RenderCore::Camera &Camera { RenderCore::GetCamera() };

        float const Sensitivity { Camera.GetSensitivity() * 0.1F };

        glm::vec3 Rotation { Camera.GetRotation() };

        Rotation.x -= OffsetX * Sensitivity;
        Rotation.y += OffsetY * Sensitivity;

        if (Rotation.y > 89.F)
        {
            Rotation.y = 89.F;
        }
        else if (Rotation.y < -89.F)
        {
            Rotation.y = -89.F;
        }

        Camera.SetRotation(Rotation);
    }
    else
    {
        glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    LastCursorPosX = NewCursorPosX;
    LastCursorPosY = NewCursorPosY;
}

void luGUI::GLFWCursorPositionCallback(GLFWwindow *const Window, double const NewCursorPosX, double const NewCursorPosY)
{
    g_IsPressingLeft = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_RELEASE;
    g_IsPressingRight = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_RELEASE;

    if (IsImGuiInitialized())
    {
        ImGuiGLFWUpdateMouse();
    }

    if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !g_CanMovementWindow && !g_IsPressingLeft)
    {
        g_CanMovementWindow = IsImGuiInitialized() && !ImGui::IsAnyItemHovered();
    }
    else if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        g_CanMovementWindow    = false;
        g_IsResizingMainWindow = false;
    }

    if (g_ViewportControlsCamera)
    {
        g_CanMovementCamera = g_ViewportHovering && g_IsPressingRight;
    }
    else
    {
        g_CanMovementCamera = g_IsPressingRight;
    }

    MovementWindow(Window, NewCursorPosX, NewCursorPosY);
    MovementCamera(Window, NewCursorPosX, NewCursorPosY);
}

void luGUI::GLFWCursorScrollCallback([[maybe_unused]] GLFWwindow *const Window, [[maybe_unused]] double const OffsetX, double const OffsetY)
{
    if (IsImGuiInitialized() && ImGui::IsAnyItemHovered())
    {
        return;
    }

    RenderCore::Camera &Camera = RenderCore::GetCamera();
    float const Zoom   = static_cast<float>(OffsetY) * 0.1f;
    Camera.SetPosition(Camera.GetPosition() + Camera.GetFront() * Zoom);
}

void luGUI::InstallGLFWCallbacks(GLFWwindow *const Window, bool const InstallClose)
{
    if (InstallClose)
    {
        glfwSetWindowCloseCallback(Window, &GLFWWindowCloseRequestedCallback);
    }

    glfwSetFramebufferSizeCallback(Window, &GLFWWindowResizedCallback);
    glfwSetKeyCallback(Window, &GLFWKeyCallback);
    glfwSetCursorPosCallback(Window, &GLFWCursorPositionCallback);
    glfwSetScrollCallback(Window, &GLFWCursorScrollCallback);
}

void luGUI::SetViewportControlsCamera(bool const Value)
{
    g_ViewportControlsCamera = Value;
}

bool luGUI::ViewportControlsCamera()
{
    return g_ViewportControlsCamera;
}

void luGUI::SetViewportHovering(bool const Value)
{
    if (!g_IsPressingRight)
    {
        g_ViewportHovering = Value;
    }
}
