// Author: Lucas Vilas-Boas
// Year : 2024
// Repo : https://github.com/lucoiso/luGUI

module;

#include "luGUIModule.hpp"

export module luGUI.UserInterface.Controls.Window;

import luGUI.Integrations.GLFWHandler;

import luGUI.UserInterface.Controls.Control;
import luGUI.UserInterface.Controls.Window.Flags;

namespace luGUI
{
    export class LUGUIMODULE_API Window : public Control
    {
        bool                m_PendingClose{false};
        InitializationFlags m_Flags{};
        GLFWHandler         m_GLFWHandler{};
        std::uint16_t       m_Width{};
        std::uint16_t       m_Height{};
        strzilla::string    m_Title{};

    public:
        Window();

        Window(Window const &) = delete;
        Window &operator=(Window const &) = delete;
        ~Window() override = default;

        bool Initialize(std::uint16_t Width, std::uint16_t Height, strzilla::string_view Title, InitializationFlags Flags = InitializationFlags::NONE);
        void RequestClose();
        void Shutdown();
        [[nodiscard]] bool IsOpen() const;

        void SetIcon(strzilla::string_view const& Path) const;
        [[nodiscard]] inline InitializationFlags GetInitializationFlags() const
        {
            return m_Flags;
        }

        void SetPosition(std::int32_t X, std::int32_t Y) const;
        void SetTitle(strzilla::string_view const& Title) const;
        [[nodiscard]] strzilla::string GetTitle() const;

        #ifdef _WIN32
        /** Experimental and unstable */
        void SetAsChildOf(::HWND ParentHandle);
        #endif // _WIN32

        virtual void PollEvents();

    protected:
        void SetRendererRequiredCallbacks();
        void Draw();

        virtual void Refresh() override;
    };
} // namespace luGUI
