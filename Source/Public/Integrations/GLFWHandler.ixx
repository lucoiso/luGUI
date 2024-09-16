// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/vulkan-renderer

module;

#ifdef GLFW_INCLUDE_VULKAN
    #undef GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

export module luGUI.Integrations.GLFWHandler;

import luGUI.UserInterface.Controls.Window.Flags;

namespace luGUI
{
    export class GLFWHandler final
    {
        GLFWwindow *m_Window { nullptr };

    public:
        [[nodiscard]] bool Initialize(std::uint16_t, std::uint16_t, strzilla::string_view, InitializationFlags);
        void               Shutdown();

        [[nodiscard]] GLFWwindow *GetWindow() const;
        [[nodiscard]] bool        IsOpen() const;
    };
} // namespace luGUI
